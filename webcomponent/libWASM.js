import { LitElement } from 'lit';
import modProm from './libwasm.js';

/**
 * libwasm WASM loader
 *
 * @customElement
 */
export class LibWASM extends LitElement {
  static get properties() {
    return {
      libWASM: { type: Object }
    }
  }

  constructor() {
    super();
    console.log('constructor')
    modProm().then((mod)=>{
      this.libWASM = mod; // for rendered wasm that delay
      this.WASMReady();
    })
  }

  /// overload this to execute something when the WASM has finished compiling
  WASMReady(){
    console.log('LibWASM.libWASM module compiled and ready to go.')
    let test = new this.libWASM.Test;
    test.sayHello();
  }
}

window.customElements.define('libwasm-', LibWASM);
