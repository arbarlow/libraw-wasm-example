import React, {useEffect} from 'react';
import LibRaw from "./raw"


const App = () => {

  useEffect(() => {
    LibRaw().then(function(Module) {
      const inputElement = document.getElementById("input");
      inputElement.addEventListener("change", handleFiles, false);

      function handleFiles() {
        var fr = new FileReader();
        fr.onload = function() {
          var d = fr.result;
          var array = new Int8Array(d);

          var buf = Module._malloc(array.length*array.BYTES_PER_ELEMENT);
          Module.HEAPU8.set(array, buf);

          Module.process_image(buf, array.length*array.BYTES_PER_ELEMENT)

          const resultPointer = Module.get_result_pointer();
          const resultSize = Module.get_result_size();

          console.log({ resultPointer, resultSize });
          const resultView = new Uint8Array(Module.HEAP8.buffer, resultPointer, resultSize);
          const result = new Uint8Array(resultView);

          const canvas = document.getElementById('viewer');
          canvas.width  = 3984;
          canvas.height = 2660;
          const ctx = canvas.getContext('2d');

          var imageData = ctx.createImageData(3984, 2660);   /// create a canvas buffer (RGBA)
          var data = imageData.data;                   /// view for the canvas buffer
          var len = data.length;                       /// length of buffer
          var i = 0;                                   /// cursor for RGBA buffer
          var t = 0;                                   /// cursor for RGB buffer

          for(; i < len; i += 4) {
            data[i]     = result[t];     /// copy RGB data to canvas from custom array
            data[i + 1] = result[t + 1];
            data[i + 2] = result[t + 2];
            data[i + 3] = 255;           /// remember this one with createImageBuffer

            t += 3;
          }

          ctx.putImageData(imageData, 0, 0);

        };

        fr.readAsArrayBuffer(inputElement.files[0]);
      }


      // this is reached when everything is ready, and you can call methods on Module
    });

  }, []);


  return (
    <div>
    <div className="App">
      <input type="file" id="input" />
    </div>
    <div>
    <canvas id="viewer"></canvas>
    </div>
    </div>
  );
}

export default App;
