let testValue = 125
function myYRangeFunction(range) {
    // TODO implement your calculation using range.min and range.max
    var min = 0;
    var max = 250;
    return {min: min, max: max};
    
  }

  
  var chart = new SmoothieChart({scrollBackwards:true,millisPerPixel:30,tooltipLine:{strokeStyle:'#bbbbbb'},yRangeFunction:myYRangeFunction}),
      canvas = document.getElementById('smoothie-chart'),
      series = new TimeSeries();
  
      setInterval(function() {
        series.append(Date.now(), testValue);
        if(Math.random() > 0.5 && testValue == 125)
            { 
        testValue += 125;
        if(testValue > 250)
            testValue = 125;
        } else 
        testValue = 125;

      }, 1000);
      
  chart.addTimeSeries(series, {lineWidth:1.7,strokeStyle:'#00ff00',interpolation:'bezier'});
  chart.streamTo(canvas, 1000);