#include "application.h"

int main(void)
{
  APP_Initialize();

  while(1)
  {
    APP_Tasks();
  }
  return 0;
}
