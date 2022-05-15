# \<vector-synth\>

Vector graphic audio synthesiser.

Interprets vector graphic waveforms and generates synthesised audio. You may specify the following :
* audio duration
* The waveform (with its own specified y range)
* Oscillation frequencies of the waveform (can be multiple base oscillation frequencies).
* The volume envelope (with specified y range)
* The pitch envelope (with specified y range)

Both nodejs and webcomponent runtimes.

# repo

https://github.com/flatmax/vector-synth

# nodejs

```
VectorSynthTestNodeTest.js
```

# webcomponent

## Example instantiation of this element (from demo/index.html) :

```
<vector-synth></vecotr-synth>
```

### Install modules

```
npm i
```

### Viewing Your Element

```
polymer serve
```
