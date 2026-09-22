# Snake Game trên STM32H563ZI

Trò chơi rắn chạy bare-metal trên vi điều khiển **STM32H563ZI** với màn hình TFT
**ILI9806G 4.3" 800×480**, board tự thiết kế. Toàn bộ driver LCD, thư viện đồ họa
và logic game đều tự viết — không dùng thư viện đồ họa ngoài, không dùng RTOS.

> 📌 Trạng thái: **đang phát triển — Giai đoạn 0 (chuẩn bị)**.
> Kế hoạch chi tiết từ A→Z nằm ở [Plan/plan.md](Plan/plan.md).

---

## Tính năng

- Rắn di chuyển theo lưới 40×22 ô, ăn mồi để dài ra, tốc độ tăng dần theo điểm
- Mồi thưởng (+50đ) có giới hạn thời gian, xuất hiện sau mỗi 5 mồi thường
- HUD thời gian thực: điểm, kỷ lục, độ dài, mức tốc độ
- Các màn hình: Splash → Menu → Chọn độ khó → Đếm ngược → Chơi → Tạm dừng → Game Over
- 3 mức độ khó: `EASY` / `NORMAL` / `HARD` (có tường chướng ngại)
- Âm thanh buzzer theo sự kiện (PWM, phát nhạc không chặn) + 3 LED báo trạng thái
- Lưu điểm cao vào Flash nội — giữ được sau khi mất nguồn

## Phần cứng

| Hạng mục | Chi tiết |
|---|---|
| MCU | STM32H563ZI — Cortex-M33 @ 250 MHz, 2 MB Flash, 640 KB SRAM |
| Màn hình | ILI9806G 4.3" 800×480, RGB565, giao tiếp 8080-II 16-bit |
| Kết nối LCD | FMC NOR/SRAM Bank1, sub-bank **NE3** (base `0x68000000`) |
| Điều khiển | 5 nút: UP / DOWN / LEFT / RIGHT / OK *(hỗ trợ được cả cấu hình 2–4 nút)* |
| Phản hồi | 3 LED (đỏ / xanh / vàng) + buzzer thụ động (PWM) |
| Debug | SWD + UART 115200-8-N-1 cho `printf` |

> ⚠️ Board là **thiết kế riêng**, nên bảng đấu nối chân phải lấy từ schematic và điền vào
> `Doc/pinout.md` trước khi mở CubeMX. Xem mục §1 của plan.

## Toolchain

- **STM32CubeMX** ≥ 6.12 + gói MCU Package cho STM32H5
- **STM32CubeIDE** ≥ 1.15 *(hoặc arm-none-eabi-gcc + make + VSCode + Cortex-Debug)*
- **STM32CubeProgrammer** để nạp / cứu board
- **gcc** trên PC — để chạy bộ test logic game ở `Test/`

---

## Kiến trúc phần mềm

```
app/      app_main.c, app_state.c        State machine: BOOT→MENU→PLAY→PAUSE→OVER
game/     snake.c, board.c               ★ THUẦN C, KHÔNG include HAL → test được trên PC
render/   render.c, gfx.c, font8x16.c    Vẽ ô, text, HUD, overlay
driver/   ili9806g.c                     Init sequence, set window, push pixel
bsp/      bsp_fmc/button/led/buzzer/log  Cầu nối tới HAL
Core/ Drivers/                           CubeMX sinh ra — KHÔNG sửa tay
```

**Nguyên tắc vàng:** lớp `game/` không phụ thuộc HAL, nhờ vậy toàn bộ luật chơi được
test bằng gcc trên máy tính trước khi nạp lên board.

### Hai quyết định kiến trúc quan trọng

1. **Không có framebuffer toàn màn hình.** 800×480×2 byte = 750 KB > 640 KB SRAM của chip.
   Giải pháp: ghi thẳng vào GRAM của LCD và **chỉ vẽ lại các ô thay đổi** — mỗi tick chỉ có
   3–4 ô đổi (đầu mới, đuôi cũ, mồi mới), tức ~1200 pixel thay vì 384.000. Tổng RAM game < 6 KB.
2. **MPU phải đặt vùng `0x68000000` là Device-nGnRE, Execute-Never.** Nếu bỏ qua, cache và
   write-buffer của Cortex-M33 sẽ gộp hoặc đổi thứ tự các lần ghi → LCD nhiễu loạn, với triệu
   chứng kinh điển là *chạy step-by-step thì đúng, chạy full speed thì hỏng*.

> STM32H5 **không có LTDC và không có DMA2D** như dòng H7 — mọi pixel do CPU hoặc GPDMA đẩy.
> Thiết kế "chỉ vẽ ô bẩn" ở trên khiến điều đó hoàn toàn không thành vấn đề.

### Vòng lặp chính

```
main() → HAL_Init → SystemClock_Config → MX_*_Init → App_Init
while (1) {
    evt = Button_PopEvent();        // hàng đợi, do ISR TIM7 (5 ms) đẩy vào
    App_HandleEvent(evt);           // state machine
    if (tick_flag) App_Tick();      // cờ do ISR TIM6 (10 ms) đặt
    Render_Flush();                 // vẽ các ô bẩn
}
```

**ISR chỉ đặt cờ hoặc đẩy vào queue — tuyệt đối không vẽ LCD trong ngắt, và không `HAL_Delay()`
trong đường chạy của game.**

---

## Cấu trúc thư mục

```
SnakeGame/
├─ Plan/plan.md        Kế hoạch chi tiết 10 giai đoạn (tài liệu gốc của dự án)
├─ Doc/                Datasheet, schematic, pinout.md, notes.md (nhật ký lỗi)
├─ CubeMX/             SnakeGame.ioc — "source code" của phần cấu hình, có commit
├─ Core/ Drivers/      CubeMX sinh ra
├─ App/                ★ Code của dự án: bsp / driver / render / game / app
├─ Test/               Bộ test logic game chạy trên PC bằng gcc
└─ README.md
```

## Build & nạp

1. Mở `CubeMX/SnakeGame.ioc` → **GENERATE CODE** (mỗi khi đổi cấu hình)
2. Mở project bằng STM32CubeIDE → **Build** (yêu cầu 0 error, 0 warning với `-Wall -Wextra`)
3. Nạp qua ST-LINK: Run / Debug trong IDE, hoặc dùng STM32CubeProgrammer với file `.elf`

Chạy bộ test logic game trên máy tính (không cần board):

```bash
make -C Test && ./Test/test_snake      # kỳ vọng in ra: ALL TESTS PASSED
```

Log chạy thực tế xem qua UART @115200 (`printf` đã được retarget).

## Điều khiển

| Nút | Trong menu | Khi chơi |
|---|---|---|
| UP / DOWN | Chọn mục | Đổi hướng |
| LEFT / RIGHT | Đổi giá trị | Đổi hướng |
| OK | Xác nhận | Tạm dừng / tiếp tục — **giữ** để về menu |

Hàng đợi input giữ 2 lệnh, nên bấm nhanh hai hướng liên tiếp trong cùng một tick vẫn ăn đủ.

## Luật chơi

- Rắn khởi đầu dài 3, ở giữa sân, hướng sang phải
- Ăn mồi thường **+10 điểm** và dài thêm 1 ô · Mồi thưởng **+50 điểm**, tồn tại 8 giây
- Chết khi đâm tường hoặc đâm thân mình — nhưng **đi vào ô đuôi đang rời đi là hợp lệ**
- Không cho quay đầu 180°
- Tốc độ: `tick_ms = max(80, 300 - (score / 50) * 20)` → nhanh dần từ 300 ms xuống 80 ms
- Thắng khi phủ kín 880 ô của sân

---

## Lộ trình

| GĐ | Nội dung | Thời lượng | Mốc | Trạng thái |
|---|---|---|---|---|
| 0 | Chuẩn bị, kiểm kê phần cứng | 0,5 ngày | | ☐ |
| 1 | Cấu hình CubeMX | 1 ngày | | ☐ |
| 2 | Bring-up LED / buzzer / nút / UART / timer | 1 ngày | **M1** | ☐ |
| 3 | Driver LCD ILI9806G ⚠️ khó nhất | 2–3 ngày | **M2** | ☐ |
| 4 | Thư viện đồ họa + font 8×16 | 1 ngày | | ☐ |
| 5 | Lõi game + test trên PC | 1,5 ngày | **M3** | ☐ |
| 6 | Lớp input | 0,5 ngày | | ☐ |
| 7 | Render theo ô bẩn | 1 ngày | **M4 — chơi được!** | ☐ |
| 8 | Âm thanh + LED | 0,5 ngày | | ☐ |
| 9 | Menu, màn hình, lưu điểm | 1 ngày | | ☐ |
| 10 | Tối ưu & hoàn thiện | 1 ngày | **M5** | ☐ |

**≈ 12 ngày công** (~4–5 tuần nếu làm buổi tối). Rủi ro lớn nhất tập trung ở Giai đoạn 3.

## Chỉ tiêu hiệu năng

| Chỉ số | Mục tiêu |
|---|---|
| `LCD_Clear()` toàn màn hình | < 30 ms |
| Vẽ 1 ô 20×20 px | < 50 µs |
| 1 lần `Render_DirtyCells` (4 ô) | < 500 µs |
| Tải CPU lúc chơi (tick 80 ms) | < 2 % |

Đo bằng `DWT->CYCCNT` (250 MHz → 1 cycle = 4 ns), ghi số liệu vào `Doc/notes.md`.

## Khi màn hình không lên — kiểm tra theo đúng thứ tự này

1. **Đọc được Device ID `0x98 0x06` chưa?** Chưa được thì đừng làm gì tiếp — mọi lỗi về sau
   đều bắt nguồn từ đây (bảng chẩn đoán đầy đủ ở §8.2 của plan).
2. **`LCD_DATA_ADDR` có khớp chân RS không?** Công thức `0x68000000 + (1 << (x + 1))` với
   `FMC_Ax` là chân nối RS. Ví dụ RS nối A18 → `0x68080000`. Sai chỗ này là lỗi phổ biến số 1.
3. **Đã cấu hình MPU cho vùng FMC chưa?** Debug từng dòng thì đúng mà chạy full speed thì hỏng
   → 99% là lỗi này.
4. **Chuỗi init có đúng panel không?** Đọc ID OK nhưng ảnh mờ / sai màu / chỉ hiện nửa màn →
   xin file init của **đúng module** từ nơi bán; phần gamma/GIP là đặc thù từng panel.

## Quy ước Git

```
main                 ← chỉ nhận code đã chạy được trên board
 └─ feat/cubemx-config · feat/bsp-bringup · feat/lcd-driver · feat/game-core
    feat/render · feat/audio-led · feat/menu-highscore · perf/optimize
```

- Commit: `<loại>(<phạm vi>): <mô tả>` — loại ∈ `feat | fix | perf | docs | chore | test`
- Gắn tag ở mỗi milestone: `git tag -a M2-lcd-ok -m "Driver LCD chạy đúng 4 test pattern"`
- Mọi code tự viết nằm trong `App/`; trong `main.c` chỉ để **lời gọi hàm** giữa các khối
  `USER CODE BEGIN/END` để CubeMX regenerate không xóa mất
- Mỗi lần vượt qua một lỗi khó → ghi ngay vào `Doc/notes.md`: triệu chứng → nguyên nhân → cách sửa

## Tài liệu

- [Plan/plan.md](Plan/plan.md) — kế hoạch đầy đủ: ràng buộc kỹ thuật, cấu hình CubeMX từng bước,
  chuỗi init ILI9806G, bộ test, tiêu chí nghiệm thu, bảng rủi ro
- `Doc/pinout.md` — bảng đấu nối chân (điền từ schematic)
- `Doc/notes.md` — nhật ký lỗi và cách khắc phục
