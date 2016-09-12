#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <iostream>

class FileUtils {
 public:
  // TODO: Move to implementation file
  template <std::size_t x, std::size_t y, std::size_t z>
  static void SaveImage(const char* imgName,
                       int imgWidth, int imgHeight,
                       float (&image)[x][y][z]) {
    FILE* fp = fopen(imgName, "wb"); /* b - binary mode */
    (void)fprintf(fp, "P6\n%d %d\n255\n", imgWidth, imgHeight);
    for (int i = 0; i < imgWidth; i++ ) {
      for (int n = 0; n < imgHeight; n++) {
        static unsigned char color[3];
        color[0] = image[i][n][0]; // red
        color[1] = image[i][n][1]; // green
        color[2] = image[i][n][2]; // blue
        (void)fwrite(color, 1, 3, fp);
      }
    }
  }
};

#endif // FILE_UTILS_H
