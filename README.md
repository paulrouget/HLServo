## Servo on Hololens.

Very much WIP.

How to compile and run:

- use this branch of servo: https://github.com/paulrouget/servo/tree/hl
- make sure both HLServo/ and servo/ are sibling directories.

With Visual Studio **2019**:
- Open `ServoApp.sln`
- click on *restore nugets packages* under the context menu of "Solution" (in the right panel). This will automatically download Angle which comes with libEGL.dll, necessary to build servo.

In your Visual Studio **2017** cmd prompt:
- make sure libEGL.dll is in your `%LIB%` path: `set LIB=%LIB%;c:\XXX\HLServo\packages\ANGLE.WindowsStore.2.1.13\bin\UAP\x64\`
- compile servo: `mach build -d --libsimpleservo --features raqote_backend`

With Visual Studio **2019**:
- Select emulator or local machine, select configuration (Debug or Release) and press run
- VS will look for the DLLs and .h in `../servo/target/debug|release/` (depending on the configuration you selected in VS) and copy them in the final package.

For now, it's not possible to interact with the web page.
