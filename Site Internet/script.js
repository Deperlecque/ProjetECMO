let testValue = 125 // initialisation d'une valeur de test
function myYRangeFunction(range) {
    // TODO implement your calculation using range.min and range.max
    var min = 0;
    var max = 250;
    return {min: min, max: max};
    
  }
// --- Simulation de données ECG ---
const pqrstWaveform = [
    0.05, 0.07, 0.1, 0.12, 0.12, 0.1, 0.07, 0.05, // P  (8)
    ...Array(7).fill(0), //  PR (7)
    -0.07, -0.12, // Q  (2)
    0, 0.98, 0, -0.05, // R  (pic) (4)
    -0.3, // S  (1)
    ...Array(13).fill(0), //  ST (13)
    0, 0.05, 0.1, 0.1, 0.125, 0.15, 0.15, 0.175, 0.2, 0.2, 0.2, 
    0.2, 0.175, 0.15, 0.15, 0.125, 0.1, 0.1, 0.05, 0, // T  (20)
    0.02, 0.02, // U  (2)
];

let BPM = 70; // BPM initial
const INITIAL_BPM = 70; 
const INTERVAL_MS = 20; // Intervalle de mise à jour (en ms)
const MIN_BPM = 61;     
const MAX_BPM = 78;    

let pqrstCycle = [];
let ecgCycleIndex = 0;

// --- Simulation de données SpO2 ---
const spo2BaseWaveform = [
    0.1, 0.3, 0.6, 0.8, 0.9, 0.7, 0.5, 0.4, // Montée (8)
    0.35, 0.3, 0.32, 0.35, 0.3, // (5)
    0.25, 0.2, 0.15, 0.1, 0.05 // Descente (5)
];

let spo2Cycle = [];
let spo2CycleIndex = 0;
let currentSpo2Value = 98; // SpO2 en %

// pour afficher les valeurs
const bpmDisplay = document.getElementById('bpmDisplay');
const spo2Display = document.getElementById('spo2Display');

// Pour faire l'ECG + SpO2  avec le BPM
function createFullCycles(currentBpm) {
    const pointsPerBeat = Math.round((60000 / currentBpm) / INTERVAL_MS);

    // ECG Cycle
    const ecgEspacementPoints = Math.max(1, pointsPerBeat - pqrstWaveform.length);
    const ecgPauseArray = Array(ecgEspacementPoints).fill(0);
    pqrstCycle = [...pqrstWaveform, ...ecgPauseArray];

    // SpO2 Cycle
    const spo2EspacementPoints = Math.max(1, pointsPerBeat - spo2BaseWaveform.length);
    const spo2PauseArray = Array(spo2EspacementPoints).fill(0);
    spo2Cycle = [...spo2BaseWaveform, ...spo2PauseArray];

}

// pour créer les 1er cycles
createFullCycles(BPM);

// pour obtenir la prochaine valeur de l'ECG
function getNextEcgValue() {
    const noise = (Math.random() - 0.5) * 0.02;
    let value = pqrstCycle[ecgCycleIndex] + noise;

    ecgCycleIndex++;
    if (ecgCycleIndex >= pqrstCycle.length) {
        ecgCycleIndex = 0;
        let bpmChange = (Math.random() - 0.5) * 10;
        BPM = Math.round(BPM + bpmChange);
        BPM = Math.max(MIN_BPM, Math.min(MAX_BPM, BPM));

        // Concerve le bpm entre 61 et 78 (BPM normal au repo)
        if (BPM < MIN_BPM + 10 || BPM > MAX_BPM - 20) {
            if (Math.random() < 0.3) { // 30% chance de revenir vers la normale
                 BPM = Math.round(BPM + (INITIAL_BPM - BPM) * 0.1); 
            }
        }
        createFullCycles(BPM);
        value = pqrstCycle[ecgCycleIndex] + noise; 
    }
    return value;
}


// pour obtenir la prochaine valeur de SpO2
function getNextSpo2WaveformValue() {
    const noise = (Math.random() - 0.5) * 0.01;
    let value = spo2Cycle[spo2CycleIndex] + noise;

    spo2CycleIndex++;
    if (spo2CycleIndex >= spo2Cycle.length) {
        spo2CycleIndex = 0;
        value = spo2Cycle[spo2CycleIndex] + noise; 
    }
    return value;
}

// pour simuler la mise à jour de SpO2
function updateSpo2Percentage() {
    if (Math.random() < 0.005) {
        const change = Math.random() < 0.5 ? -1 : 1;
        currentSpo2Value += change;
        currentSpo2Value = Math.max(90, Math.min(100, currentSpo2Value));
    }
}


// --- Configuration de smoothiechart pour ECG ---
const ecgSmoothie = new SmoothieChart({
    grid: {
        strokeStyle: 'rgba(0, 128, 0, 0.7)',
        fillStyle: 'rgb(0, 0, 0)',
        lineWidth: 1,
        millisPerLine: 500,
        verticalSections: 6,
    },
    labels: {
        fillStyle: 'rgb(0, 255, 0)',
        fontSize: 12,
        precision: 2,
        disabled: true
    },
    maxValue: 1.2,
    minValue: -0.6,
    responsive: true,
    millisPerPixel: 6, // Vitesse de défilement
    interpolation: 'bezier',
});

const ecgTimeSeries = new TimeSeries();
ecgSmoothie.addTimeSeries(ecgTimeSeries, {
    strokeStyle: 'rgb(0, 255, 0)',
    lineWidth: 2.5
});

const ecgCanvas = document.getElementById('ecgChart');
ecgSmoothie.streamTo(ecgCanvas, 100); // Délai de rafraîchissement graphique en ms

// --- Configuration de smoothiechart pour SpO2 ---
const spo2Smoothie = new SmoothieChart({
    grid: {
        strokeStyle: 'rgba(0, 225, 255, 0.4)',
        fillStyle: 'rgb(0, 0, 0)',
        lineWidth: 1,
        millisPerLine: 500,
        verticalSections: 4,
    },
    labels: {
        fillStyle: 'rgb(0, 225, 255)',
        fontSize: 12,
        precision: 2,
        disabled: true
    },
    maxValue: 1.1,
    minValue: -0.1,
    responsive: true,
    millisPerPixel: 6, // Vitesse de défilement
    interpolation: 'bezier',
});

const spo2TimeSeries = new TimeSeries();
spo2Smoothie.addTimeSeries(spo2TimeSeries, {
    strokeStyle: 'rgb(0, 225, 255)',
    lineWidth: 2.5
});

const spo2Canvas = document.getElementById('spo2Chart');
spo2Smoothie.streamTo(spo2Canvas, 100);


// --- Démarrage de la simulation ---
function updateDisplays() {
    if (bpmDisplay) {
        bpmDisplay.innerText = `FC: ${BPM} bpm`;
    }
    if (spo2Display) {
        spo2Display.innerText = `SpO₂: ${currentSpo2Value} %`;
    }
}

setInterval(function() {
    const timestamp = Date.now();

    const ecgValue = getNextEcgValue();
    ecgTimeSeries.append(timestamp, ecgValue);

    const spo2WaveformPoint = getNextSpo2WaveformValue();
    spo2TimeSeries.append(timestamp, spo2WaveformPoint);

    updateSpo2Percentage();
    updateDisplays();

}, INTERVAL_MS);

// Initialiser l'affichage des valeurs
updateDisplays();

// Gros problème avec les résolution autre que 1080p, 1440p, 4k en 16/9 sans cette partie.
// le graph disparait ou grandit en continue sans cette partie :( 
function resizeCanvases() {
    const canvases = document.querySelectorAll('canvas');
    canvases.forEach(canvas => {
    });
}

window.addEventListener('resize', resizeCanvases);

  
  var chart = new SmoothieChart({scrollBackwards:true,millisPerPixel:30,
    tooltipLine:{strokeStyle:'#bbbbbb'},yRangeFunction:myYRangeFunction}),
      canvas = document.getElementById('smoothie-chart'),
      series = new TimeSeries();
  
      setInterval(function() {
        // Utilisation d'une valeur de test pour afficher sur le graphique
        // une fois sur deux, le graphique monte à 125 puis redescend
        series.append(Date.now(), testValue);
        if(Math.random() > 0.5 && testValue == 125)
            { 
        testValue = 250;
        testValue = 125;
        } else 
        testValue = 125;
      }, 1000);
      
  chart.addTimeSeries(series, {lineWidth:1.7,strokeStyle:'#00ff00',
    interpolation:'bezier'});
  chart.streamTo(canvas, 1000);