const LOADING_CONTAINER_ID = "loading-container";
const RUN_BUTTON_CONTAINER_ID = "run-button-container";
const EMULATOR_CONTAINER_ID = "emulator-container";

const CANVAS_ID = "canvas";

var Module = {
    onRuntimeInitialized: function() {
        let loadingContainer = document.getElementById(LOADING_CONTAINER_ID);
        loadingContainer.style.display = 'none';
        let runButtonContainer = document.getElementById(RUN_BUTTON_CONTAINER_ID);
        runButtonContainer.style.display = 'block';
    },
    canvas: (function() {
        let canvas = document.getElementById(CANVAS_ID);
        return canvas;
    })()
};

function run_emulator() {
    let runButtonContainer = document.getElementById(RUN_BUTTON_CONTAINER_ID);
    runButtonContainer.style.display = 'none';
    let emulatorContainer = document.getElementById(EMULATOR_CONTAINER_ID);
    emulatorContainer.style.display = 'block';

    Module.ccall('run', null, null, null, {async: true});
}