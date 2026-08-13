import sys
try:
    from PIL import Image
except ImportError:
    print("Error: Pillow is not installed. Run 'pip install Pillow'")
    sys.exit(1)

if len(sys.argv) != 4:
    print("Usage: img_converter.py <input_img> <output_c> <variable_name>")
    sys.exit(1)

input_path = sys.argv[1]
output_path = sys.argv[2]
var_name = sys.argv[3]

# open and resize the image
img = Image.open(input_path).convert('RGB')
img = img.resize((128, 128))

img = img.quantize(colors=256,
                   method=Image.Quantize.MEDIANCUT,
                   dither=Image.Dither.FLOYDSTEINBERG).convert('RGB')

get_pixels = getattr(img, 'get_flattened_data', img.getdata)

with open(output_path, 'w') as f:
    f.write('#include "lvgl.h"\n\n')

    f.write(f'const uint8_t {var_name}_map[] = {{\n')
    for idx, (r_val, g_val, b_val) in enumerate(get_pixels()):

        # Scale to 5-bit Red, 6-bit Green, 5-bit Blue
        r5 = r_val >> 3
        g6 = g_val >> 2
        b5 = b_val >> 3

        byte_0 = (b5 << 3) | (g6 >> 3)
        byte_1 = ((g6 & 0x07) << 5) | r5

        f.write(f'0x{byte_0:02x}, 0x{byte_1:02x}, ')
        if idx % 8 == 7:
            f.write('\n')
    f.write('};\n\n')

    f.write(f'const lv_image_dsc_t {var_name} = {{\n')
    f.write('  .header.cf = LV_COLOR_FORMAT_RGB565,\n')
    f.write('  .header.w = 128,\n')
    f.write('  .header.h = 128,\n')
    f.write('  .data_size = 128 * 128 * 2,\n')
    f.write(f'  .data = {var_name}_map,\n')
    f.write('};\n')
