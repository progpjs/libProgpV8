// ../node_modules/@progp/core/index.ts
var g_nextTimerId = 1;
var g_timers = [];
globalThis.setTimeout = function(callbackFct, timeInMs, ...params) {
  if (!callbackFct)
    return -1;
  const timerId = g_nextTimerId++;
  g_timers[timerId] = true;
  progpCallAfterMs(timeInMs, () => {
    let timerState = g_timers[timerId];
    if (timerState) {
      delete g_timers[timerId];
      callbackFct.call(globalThis, params);
    }
  });
  return timerId;
};
function setIntervalAux(callbackFct, timeInMs, params, timerId) {
  progpCallAfterMs(timeInMs, () => {
    let timerState = g_timers[timerId];
    if (timerState) {
      callbackFct.call(globalThis, params);
      setIntervalAux(callbackFct, timeInMs, params, timerId);
    }
  });
}
globalThis.setInterval = function(callbackFct, timeInMs, ...params) {
  if (!callbackFct)
    return -1;
  const timerId = g_nextTimerId++;
  g_timers[timerId] = true;
  setIntervalAux(callbackFct, timeInMs, params, timerId);
  return timerId;
};
globalThis.clearTimeout = function(timerId) {
  delete g_timers[timerId];
};
globalThis.clearInterval = globalThis.clearTimeout;
var bckConsoleLog = globalThis.console.log;
var bckConsoleWarn = globalThis.console.warn;
var bckConsoleError = globalThis.console.error;
var bckConsoleDebug = globalThis.console.debug;
var bckConsoleInfo = globalThis.console.info;
globalThis.console.log = function(...data) {
  progpPrint(...data);
  if (bckConsoleLog)
    bckConsoleLog(...data);
};
globalThis.console.warn = function(...data) {
  progpPrint("[WARN] ", ...data);
  if (bckConsoleWarn)
    bckConsoleWarn(...data);
};
globalThis.console.error = function(...data) {
  progpPrint("[ERROR] ", ...data);
  if (bckConsoleError)
    bckConsoleError(...data);
};
globalThis.console.debug = function(...data) {
  progpPrint("[DEBUG] ", ...data);
  if (bckConsoleDebug)
    bckConsoleDebug(...data);
};
globalThis.console.info = function(...data) {
  progpPrint("[INFO] ", ...data);
  if (bckConsoleInfo)
    bckConsoleInfo(...data);
};

// index.ts
debugger;
progpPrint("d1");
debugger;
progpPrint("d2");
//# sourceMappingURL=stdin.js.map
