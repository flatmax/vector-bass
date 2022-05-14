import { LitElement } from 'lit';
import modProm from './libVS.js';

/**
 * libVS WASM loader
 *
 * @customElement
 */
export class LibVectorSynth extends LitElement {
  static get properties() {
    return {
      libVS: { type: Object }
    }
  }

  constructor() {
    super();
    console.log('constructor')
    modProm().then((mod)=>{
      this.libVectorSynth = mod; // for rendered wasm that delay
      this.WASMReady();
    })
  }

  /// overload this to execute something when the WASM has finished compiling
  WASMReady(){
    console.log('LibVectorSynth.libVectorSynth module compiled and ready to go.')
  }
}

window.customElements.define('vector-synth', LibVectorSynth);
