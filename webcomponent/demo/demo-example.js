import { LitElement, html } from 'lit';
import '../libWASM';

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

      <libwasm->
        libwasm- loading ...
      </libwasm->
      check your console for WASM output!
    `;
  }

  static get properties() {
    return {
    };
  }
}
customElements.define('demo-example', DemoExample);
