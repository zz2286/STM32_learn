# F103C8T6_hal-lib_demo

这个文件夹下存放一些使用hal库实现的demo。  
path: STM32_learn/Demos/F103C8T6_hal-lib_demo

## How to start with STM32CubeMX

- base 00_HAL_StartWith_CubeMX:
    - copy 00_HAL_StartWith_CubeMX folder.
    - rename folder name.
    - rename *.ioc filename, and open it.
- base exist project folder:
    - copy xx_HAL_xxxx folder.
    - rename folder name.
    - delete sub-folder MDK-ARM.
    - delete the unused driver or core code file if it exist.
    - rename xx_HAL_xxxx.ioc filename, and open it.
- Re-setup Keil project's [Options] after re-generation.