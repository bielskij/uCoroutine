# uCoroutine
This is a lightweight and fast coroutines implementation designated for small microcontrollers. It is easy portable and fully independent from operating system or platform.

The code bases on best fragments of FreeRTOS and RTThread libraries. The main idea was extraction of coroutine logic and make a simple library for it.

# Features.
Current implementation supports:
- coroutine priorities
- configuration on compile time
- statically/dynamically allocated objects
- standard coroutine switching methods
  - sleep
  - yield
- queue type

# Supported platforms.
The code was tested on x86, x64, avr platforms with GCC toolchain.

# Compile time configuration.
The library can be configured by ```uCoroutine/config.h``` header. Sample configuration file is available in ```uCoroutine/sample/config.h```

# Porting to other platform.
uCoroutine uses ```uCoroutine/platform.h``` header for getting definitions of platform dependent functions like:
 - methods that MUST be defined (and implemented)
   - ```uCoroutine_platform_isr_enable```
   - ```uCoroutine_platform_isr_disable```
   - ```uCoroutine_platform_getTicks```
 - methods used by dynamic allocation
   - ```uCoroutine_platform_malloc```
   - ```uCoroutine_platform_free```
 - methods used by debug submodule
   - ```uCoroutine_platform_abort```
   - ```uCoroutine_platform_printf```

Sample ```platform.h``` header is available at ```uCoroutine/sample/platform.h```
