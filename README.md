Actually exit Electron apps
============================

Usage:
```javascript
const electron_exit = require('electron_exit');

// Exit 1 second from now (even if the JavaScript context is destroyed between now and then)
electron_exit.exit(1000);
// Cancel previously issued exit
electron_exit.exitCancel();
// Absolutely exit right now
electron_exit.exit();
console.log('This will never be executed');
```

Note: issuing any new exit request also cancels the previous one, so calling `electron_exit.exit(5000)` once a second will cause the program to exit 4-5 seconds after Electron has hung / frozen.

Note: though this module can be included on any platform, it untested except on Win32.