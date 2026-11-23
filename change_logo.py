
from PIL import Image
from pathlib import Path


TARGET_HEIGHT = 175
IMAGE_DIR = Path.cwd() / "img"


def convert_png():

  if not IMAGE_DIR.is_dir():
    print("No img folder!")
    return

  files = sorted(
    [
      f for f in IMAGE_DIR.iterdir()
      if f.is_file() and f.suffix.lower() in (".png", ".jpg", ".jpeg")
    ]
  )

  if not files:
    print("No image files in img folder!")
    return

  print("\nAvailable images:\n")

  for i, file in enumerate(files, 1):
    print(f"{i}. {file.name}")

  try:
    choice = int(input("\nSelect image: "))

    if choice < 1 or choice > len(files):
      print("Incorrect number!")
      return

  except ValueError:
    print("Enter corrent number!")
    return

  png_path = files[choice - 1]

  try:
    img = Image.open(png_path).convert("RGBA")

    original_width, original_height = img.size

    new_height = TARGET_HEIGHT

    new_width = max(
      1,
      round(original_width * TARGET_HEIGHT / original_height)
    )

    img = img.resize(
      (new_width, new_height),
      Image.Resampling.LANCZOS
    )

    background = Image.new(
      "RGBA",
      img.size,
      (0, 0, 0, 255)
    )

    background.alpha_composite(img)
    img = background.convert("RGB")

    pixels = list(img.getdata())
    rgb565 = []

    for r, g, b in pixels:
      color = (
        ((r & 0xF8) << 8) |
        ((g & 0xFC) << 3) |
        (b >> 3)
      )

      rgb565.append(f"0x{color:04X}")

    output = []

    output.append("#pragma once")
    output.append("#include <Arduino.h>")
    output.append("#include <pgmspace.h>")
    output.append("")

    output.append(f"#define LOGO_WIDTH {new_width}")
    output.append(f"#define LOGO_HEIGHT {new_height}")
    output.append("")

    output.append(
      f"const uint16_t logo[{new_width * new_height}] PROGMEM = {{"
    )

    for i in range(0, len(rgb565), 12):
      line = ", ".join(rgb565[i:i + 12])
      output.append("    " + line + ",")

    output.append("};")

    output_dir = Path.cwd() / "src"
    output_dir.mkdir(exist_ok=True)

    output_path = output_dir / "logo.h"

    with open(output_path, "w", encoding="utf-8") as f:
      f.write("\n".join(output))

    print("\nLogo changed!")

  except Exception as e:
    print(f"Error: {e}")


if __name__ == "__main__":
  convert_png()
