const LOADING_CONTAINER_ID = "loading-container";
const RUN_BUTTON_CONTAINER_ID = "run-button-container";
const EMULATOR_CONTAINER_ID = "emulator-container";

const CANVAS_ID = "canvas";

const HISCORE_KEY = "Wasm-Invaders-HiScore";

var Module = {
    onRuntimeInitialized: function() {
        let loadingContainer = document.getElementById(LOADING_CONTAINER_ID);
        loadingContainer.style.display = 'none';
        let runButtonContainer = document.getElementById(RUN_BUTTON_CONTAINER_ID);
        runButtonContainer.style.display = 'block';

        let daisyUiStylesheet = document.querySelector("link");
        document.querySelector("head").appendChild(daisyUiStylesheet);
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

    let hiScore = window.localStorage.getItem(HISCORE_KEY);
    if(hiScore === null) {
        Module.ccall('run', null, null, null, {async: true});
    } else {
        Module.ccall('run', null, ['number'], [parseInt(hiScore)], {async: true});
    }
}

function reset_emulator() {
    Module.ccall('reset', null, null, null, {async: true});
}

async function export_hiscore() {
    let hiScore = await Module.ccall('exportHiScoreData', 'number', null, null, {async: true});
    localStorage.setItem(HISCORE_KEY, hiScore.toString());
}