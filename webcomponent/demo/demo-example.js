import { LitElement, html } from 'lit';
import '../LibVectorSynth';

/** Example demo element for testing element loading
*/
class DemoExample extends LitElement {
  render(){
    return html`
      <style>
        :host { display: block; }
        div[hidden] {
          display: none;
        }
        sox-audio {
          padding: 10px;
          margin: 10px 0;
          border: 1px solid #bcd
        }
      </style>

      <vector-synth>
        vector-synth loading ...
      </vector-synth>
      check your console for WASM output!
    `;
  }

  static get properties() {
    return {
    };
  }
}
customElements.define('demo-example', DemoExample);
