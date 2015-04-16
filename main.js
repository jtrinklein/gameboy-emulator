var os = require('os'),
    gcd = require('gcd-js'),
    cycles = require('./cycles.json');

var memory = [];


var cpuSpeedHz = os.cpus()[0].speed * 1000000;
var refreshRateHz = 60;
var vBlankPeriod = cpuSpeedHz / refreshRateHz;
var scanLinePeriod = vBlankPeriod / 256;
var vBlankIdlePeriod = 44 * scanLinePeriod;
var INTERRUPT_PERIOD = Math.ceil(gcd(vBlankPeriod, scanLinePeriod, vBlankIdlePeriod));
var INITIAL_PC = 0;

var counter = INTERRUPT_PERIOD;
var pc = INITIAL_PC;

var exitRequired = false;


function readMemory(addr) {
    return memory[addr];
}

function writeMemory(addr, data) {
    memory[addr] = data;
}

function coreLoop() {
    for (;;) {
        var opCode = memory[pc++];
        counter -= cycles[opCode];

        switch(opCode) {
        case 0x00://NOP
            break;
        case 0x01:
            break;
        case 0x20:
            //if action, add 4 cycles
            break;
        case 0x28:
            //if action, add 4 cycles
            break;
        case 0x30:
            //if action, add 4 cycles
            break;
        case 0x38:
            //if action, add 4 cycles
            break;
        case 0xC0:
            //if action, add 12 cycles
            break;
        case 0xC2:
            //if action, add 4 cycles
            break;
        case 0xC4:
            //if action, add 12 cycles
            break;
        case 0xC8:
            //if action, add 12 cycles
            break;
        case 0xCA:
            //if action, add 4 cycles
            break;
        case 0xCC:
            //if action, add 12 cycles
            break;
        case 0xD0:
            //if action, add 12 cycles
            break;
        case 0xD2:
            //if action, add 4 cycles
            break;
        case 0xD4:
            //if action, add 12 cycles
            break;
        case 0xD8:
            //if action, add 12 cycles
            break;
        case 0xDA:
            //if action, add 4 cycles
            break;
        case 0xDC:
            //if action, add 12 cycles
            break;
        }

        if (counter <= 0) {
            counter += INTERRUPT_PERIOD;
            // do ..cyclical.. stuff
            if (exitRequired) {
                break;
            }
        }
    }
}

console.log('Interrupt Period: ' + INTERRUPT_PERIOD + 'Hz');
