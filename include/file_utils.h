#ifndef FILE_UTILS_H
#define FILE_UTILS_H

class FileUtils {
 public:
  // TODO: Move to implementation file
  template <std::size_t x, std::size_t y, std::size_t z>
  static void SaveImage(const char* img_name,
                        int img_width, int img_height,
                        float (&image)[x][y][z]) {
    FILE* fp = fopen(img_name, "wb"); /* b - binary mode */
    (void)fprintf(fp, "P6\n%d %d\n255\n", img_width, img_height);
    for (int i = 0; i < img_width; i++ ) {
      for (int j = 0; j < img_height; j++) {
        static unsigned char color[3];
        color[0] = image[i][j][0]; // red
        color[1] = image[i][j][1]; // green
        color[2] = image[i][j][2]; // blue
        (void)fwrite(color, 1, 3, fp);
      }
    }
  }
};

#endif // FILE_UTILS_H
