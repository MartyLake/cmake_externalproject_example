#include <iostream>
#include "nifti2_io.h"

int main() {
  std::cout << "Hello World!";
  const auto nim = nifti_image_read("this_file_does_not_exist.nii", 1);
  if (nim)
    nifti_image_free(nim);
  return 0;
}
