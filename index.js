/* eslint no-empty-function:off, global-require:off */

// Exposes:
//   exit(delay_ms)
//   exitCancel()

try {
  module.exports = require('bindings')('electron-exit.node');
} catch (e) {
  module.exports = {
    exit: function () {},
    exitCancel: function () {},
  };
}
