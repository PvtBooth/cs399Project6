
// jQuery("#loading").toggle();
Promise.all([
  d3.csv("https://raw.githubusercontent.com/PvtBooth/cs399project6/master/project7/UnitTests/BOMB.csv"),
  d3.csv("https://raw.githubusercontent.com/PvtBooth/cs399project6/master/project7/UnitTests/GRAVITAS.csv"),
  d3.csv("https://raw.githubusercontent.com/PvtBooth/cs399project6/master/project7/UnitTests/MISSILE.csv"),
  d3.csv("https://raw.githubusercontent.com/PvtBooth/cs399project6/master/project7/UnitTests/SPRAY.csv"),
])
.then(function(data) {   
  main(data);
})
.catch(function(result) {
  error(result);
});

var bombData, gravitasData, missileData, sprayData;
function main(data)
{
  jQuery("#loading").toggle();
  bombData = data[0];
  gravitasData = data[1];  
  missileData = data[2];
  sprayData = data[3];
  AnalyzeUnitTest(bombData);
}

// Main Data
var playerHealth;
var enemyHealth;
var numEnemies;

var strategy = "s0";
var DPS = 0.0;
var DamageReceived = 0.0;
var enemies = [];

// Per frame stored values
var PerFrameData = []; // ENTRIES: Time, PlayerHP, EnemiesHP, TotalEnemies

function AnalyzeUnitTest(data)
{
  ResetCharts();
  PerFrameData = [];
  enemies = [];
  playerHealth = 100.0;
  enemyHealth = 0.0;
  numEnemies = 0;
  var initialDamageTime = -1.0;

  data.forEach(function(eventLog) {
  if(eventLog.LogType === "L_spawn")
  {
    numEnemies++;
    enemyHealth += 8.0;
    enemies.push({
      ID: eventLog.SourceID,
      HP: 8.0
    })
  }
  else if(eventLog.LogType === "L_damage")
  {
      var dmg = parseFloat(eventLog.Damage);
      if(eventLog.DestinationType === "PlayerType1")
      playerHealth -= dmg;
    else
    {
      enemies.forEach(enemy => 
      {
        if(enemy.ID === eventLog.DestinationID)
          enemy.HP -= dmg;
      })
      enemyHealth = GetEnemyTotalHP();
    }
    if(initialDamageTime < 0.0)
    {
      initialDamageTime = parseFloat(eventLog.Time);
      PerFrameData.push({
        Time: 0.0,
        PlayerHP: playerHealth,
        EnemiesHP: enemyHealth / 8.0,
        TotalEnemies: numEnemies
      })
    }
  }
  else if(eventLog.LogType === "L_death")
  {
    enemies.splice(enemies.indexOf(e => e.ID === e.SourceID));
    numEnemies--;
  }
  else if(eventLog.LogType === "L_time")
  {
    if(initialDamageTime < 0.0)
      return;
    PerFrameData.push({
      Time: parseFloat(eventLog.Time) - initialDamageTime,
      PlayerHP: playerHealth,
      EnemiesHP: enemyHealth / 8.0,
      TotalEnemies: numEnemies
    });
  }})
  RenderDamageChart();
}

function GetEnemyTotalHP(){
  var HP = 0.0;
  enemies.forEach(enemy => HP += enemy.HP > 0.0 ? enemy.HP : 0.0);
  return HP;
}

var margin = {top: 20, right: 20, bottom: 20, left: 20},
    padding = {top: 60, right: 60, bottom: 60, left: 60},
    outerWidth = 910,
    outerHeight = 500,
    innerWidth = outerWidth - margin.left - margin.right,
    innerHeight = outerHeight - margin.top - margin.bottom,
    width = innerWidth - padding.left - padding.right,
    height = innerHeight - padding.top - padding.bottom;

var text_margin = {top: 0, right: 0, bottom: 0, left: 0},
    text_padding = {top: 0, right: 0, bottom: 0, left: 0},
    text_outerWidth = 300,
    text_outerHeight = 25,
    text_innerWidth = text_outerWidth - text_margin.left - text_margin.right,
    text_innerHeight = text_outerHeight - text_margin.top - text_margin.bottom,
    text_width = text_innerWidth - text_padding.left - text_padding.right,
    text_height = text_innerHeight - text_padding.top - text_padding.bottom;

var percent_line_x = d3.scaleTime().range([margin.left, width]);
var percent_line_y = d3.scaleLinear().range([height, (margin.top)]);

var percent_line_xAxis;
var percent_line_yAxis;

var percent_line_x_domain;
var percent_line_y_domain;

var percent_g;

var percentline = d3.line()
    .x(function(d) { return percent_line_x(d.date); })
    .y(function(d) { return percent_line_y(d.change); });

var percent_histogram_chart = d3.select(".percentage_histogram")
    .attr("width", outerWidth)
    .attr("height", outerHeight)
    .append("g")
    .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

var percent_change_chart = d3.select(".account_percentage_change_line_chart")
    .attr("width", outerWidth)
    .attr("height", outerHeight)
    .append("g")
    .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

var shares_of_stocks_bought_chart = d3.select(".stock_bar_chart")
    .attr("width", outerWidth)
    .attr("height", outerHeight)
    .append("g")
    .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

//Text objects
var DPSText = d3.select(".DPSText")
    .attr("width", text_outerWidth)
    .attr("height", text_outerHeight)
    .append("g")
    .attr("transform", "translate(" + text_margin.left + "," + text_margin.top + ")");

var DamageReceivedText = d3.select(".DamageReceivedText")
    .attr("width", text_outerWidth)
    .attr("height", text_outerHeight)
    .append("g")
    .attr("transform", "translate(" + text_margin.left + "," + text_margin.top + ")");

var AverageFrameTimeText = d3.select(".AverageFrameTimeText")
    .attr("width", text_outerWidth)
    .attr("height", text_outerHeight)
    .append("g")
    .attr("transform", "translate(" + text_margin.left + "," + text_margin.top + ")");


var histogram_left_max = -8.0;
var histogram_right_max = 8.0;

var panningLeft = false;
var panningRight = false;
var beginningIndex = 0;
var endingIndex = 80;

var ResetCharts = function()
{
  beginningIndex = 0;
  endingIndex = 80;

  panningRight = false;
  panningLeft = false;

  //d3.select("body").selectAll("svg").remove();

  //Clear charts
  clearChart("percentage_histogram");
  percent_histogram_chart = d3.select(".percentage_histogram")
    .attr("width", outerWidth)
    .attr("height", outerHeight)
    .append("g")
    .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

  //Damage Chart
  clearChart("DamageChart");
  DamageChart = d3.select(".DamageChart")
    .attr("width", outerWidth)
    .attr("height", outerHeight)
    .append("g")
    .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

  //percent_change_chart
  clearChart("account_percentage_change_line_chart");
  percent_change_chart = d3.select(".account_percentage_change_line_chart")
    .attr("width", outerWidth)
    .attr("height", outerHeight)
    .append("g")
    .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

  //shares_of_stocks_bought_chart
  clearChart("stock_bar_chart");
  shares_of_stocks_bought_chart = d3.select(".stock_bar_chart")
    .attr("width", outerWidth)
    .attr("height", outerHeight)
    .append("g")
    .attr("transform", "translate(" + margin.left + "," +margin.top + ")");

  clearChart("DPSText");
  DPSText = d3.select(".DPSText")
    .attr("width", text_outerWidth)
    .attr("height", text_outerHeight)
    .append("g")
    .attr("transform", "translate(" + text_margin.left + "," + text_margin.top + ")");

    clearChart("DamageReceivedText");
    DamageReceivedText = d3.select(".DamageReceivedText")
    .attr("width", text_outerWidth)
    .attr("height", text_outerHeight)
    .append("g")
    .attr("transform", "translate(" + text_margin.left + "," + text_margin.top + ")");

    clearChart("AverageFrameTimeText");
    AverageFrameTimeText = d3.select(".account_percentage_AverageFrameTimeTextgain_graph")
    .attr("width", text_outerWidth)
    .attr("height", text_outerHeight)
    .append("g")
    .attr("transform", "translate(" + text_margin.left + "," + text_margin.top + ")");
}

function clearChart(chartName)
{
  $("#" + chartName).empty();
}

var changeStrategy = function(p_strategy)
{
  if(p_strategy == "s0")
  {
    AnalyzeUnitTest(bombData)
  }
  else if(p_strategy == "s1")
  {
    AnalyzeUnitTest(gravitasData)
  }
  else if(p_strategy == "s2")
  {
    AnalyzeUnitTest(missileData)
  }
  else if (p_strategy == "s3")
  {
    AnalyzeUnitTest(sprayData)
  }
}

function renderPercentHistogramChart()
{
  //Histogram graph
var histogram = d3.histogram()
    .domain([histogram_left_max, histogram_right_max])
    .thresholds(histogram_right_max * 4);

var bins = histogram(DailyPercentageChanges);

  // Scale the range of the data
var bar_x = d3.scaleLinear()
    .range([margin.left, width]);

var bar_x_axis = bar_x.domain([histogram_left_max, histogram_right_max]);

var bar_y = d3.scaleLinear()
    .range([height, margin.top]);

var bar_y_axis = bar_y.domain([0, d3.max(bins, function(d) { return d.length; })]);

 // Add the x Axis
  percent_histogram_chart.append("g")
      .attr("transform", "translate(" + (margin.left + (margin.right/2)) + ", " + (height) + ")")
      .call(d3.axisBottom(bar_x).ticks(histogram_right_max * 4));

    // text label for the x axis
  percent_histogram_chart.append("text")             
      .attr("transform",
            "translate(" + ((width/2) + margin.left + (margin.right)) + " ," + 
                           (height + 60) + ")")
      .style("text-anchor", "middle")
      .text("Percent Account Change");

    // Add the y Axis
  percent_histogram_chart.append("g")
      .attr("transform", "translate(" + (margin.left + margin.right + (margin.left / 2)) + ", " + 0 + ")")
      .call(d3.axisLeft(bar_y));

  // text label for the y axis
  percent_histogram_chart.append("text")
      .attr("transform", "rotate(-90)")
      .attr("y", 0)
      .attr("x",0 - (height/2))
      .attr("dy", "1em")
      .style("text-anchor", "middle")
      .text("Days");


   // Add the valueBar bar
   percent_histogram_chart.selectAll("rect")
      .data(bins)
      .enter().append("rect")
      .attr("class", "bar")
      .attr("x", d => bar_x_axis(d.x0) + (margin.left + (margin.right/2)))
      .attr("y", function(d) { /*console.log(d.length);*/ return bar_y_axis(d.length); })
      .attr("height", function(d) { return height - bar_y_axis(d.length); })
      .attr("width", d => Math.max(0, bar_x_axis(d.x1) - bar_x_axis(d.x0) - 1))
      .attr("fill", function(d) {return colorOfBar(d.x0);});

    percent_histogram_chart.append("text")
      .attr("x", 0 + (outerWidth/2))
      .attr("y", 0)
      .style("text-anchor", "middle")
      .text("Number of Days per Percentage Change Range");
}


// ============================ DAMAGE_CHART ====================================
var DamageChart = d3.select(".DamageChart")
    .attr("width", outerWidth)
    .attr("height", outerHeight)
    .append("g")
    .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

var xScaleDamage = d3.scaleLinear().range([margin.left, width]);
var yScaleDamage = d3.scaleLinear().range([height, (margin.top)]);
// define the line 

var PlayerHPLine = d3.line()
    .x(function(d) { return xScaleDamage(d.Time); })
    .y(function(d) { return yScaleDamage(d.PlayerHP); });

var NumEnemiesLine = d3.line()
    .x(function(d) { return xScaleDamage(d.Time); })
    .y(function(d) { return yScaleDamage(d.TotalEnemies); });

var EnemyHPLine = d3.line()
    .x(function(d) { return xScaleDamage(d.Time); })
    .y(function(d) { return yScaleDamage(d.EnemiesHP); });

function RenderDamageChart()
{
    // Damage Graph:x Time,y1 PlayerHP,y2 EnemiesHP,y3 TotalEnemies
    // Scale the range of the data
  xScaleDamage.domain(d3.extent(PerFrameData, function(d, i) { return d.Time; }));
  yScaleDamage.domain([0, d3.max(PerFrameData, function(d, i) { return d.PlayerHP; })]);

  var DamageChart_g = DamageChart.append("g")
    .attr("transform", "translate(" + padding.left + "," + padding.top + ")");

        // Add the valueline path.
  DamageChart_g.append("path")
      .data([PerFrameData])
      .attr("class", "line")
      .attr("d", PlayerHPLine)
      .attr("fill", "none")
      .attr("stroke", "green")
      .attr("stroke-width", 1.5);

  DamageChart_g.append("path")
    .data([PerFrameData])
    .attr("class", "line")
    .attr("d", NumEnemiesLine)
    .attr("fill", "none")
    .attr("stroke", "red")
    .attr("stroke-width", 1.5);

  DamageChart_g.append("path")
    .data([PerFrameData])
    .attr("class", "line")
    .attr("d", EnemyHPLine)
    .attr("fill", "none")
    .attr("stroke", "orange")
    .attr("stroke-width", 1.5);

    // Add the x Axis
  DamageChart_g.append("g")
    .attr("class", "x axis")
    .attr("transform", "translate(0," + height + ")")
    .call(d3.axisBottom(xScaleDamage));

    // text label for the x axis
  DamageChart_g.append("text")             
      .attr("transform",
            "translate(" + ((width/2) + margin.left) + " ," + 
                           (height + (margin.bottom) + margin.top) + ")")
      .style("text-anchor", "middle")
      .text("Time");

    // Add the y Axis
  DamageChart_g.append("g")
    .attr("class", "y axis")
    .attr("transform", "translate(" + margin.left + ",0)")
    .call(d3.axisLeft(yScaleDamage));

  // text label for the y axis
  DamageChart.append("text")
      .attr("transform", "rotate(-90)")
      .attr("y", 0 + (margin.left/4))
      .attr("x",0 - (((height)/1.8) + margin.top + margin.bottom) )
      .attr("dy", "1em")
      .style("text-anchor", "middle")
      .text("Damage");

  DamageChart.append("text")
      .attr("x", 0 + (outerWidth/2))
      .attr("y", 0)
      .style("text-anchor", "middle")
      .text("Unit Test Analysis");
}

var clearIntervalIDRight;
var clearIntervalIDLeft;
function panLeft()
{
  panningLeft = true;

  clearIntervalIDLeft = setInterval(function(){panning();}, 100);
}

function stopPanLeft()
{
  panningLeft = false;

  clearInterval(clearIntervalIDLeft);

  console.log("Clearing left");
}

function panRight()
{
  panningRight = true;

  clearIntervalIDRight = setInterval(function(){panning();}, 100);
}

function stopPanRight()
{
  panningRight = false;

  clearInterval(clearIntervalIDRight);

  console.log("Clearing right");
}

var percent_line_domain_beginning;
var percent_line_domain_end;

var renderPercentChangeArray = [];

function initializeEndingIndex(value)
{
  if(DailyPercentageChangesWithDates.length <= value)
  {
    endingIndex = Math.round(value / 2);

    initializeEndingIndex(endingIndex);
  }
}

function panning()
{
  if(panningRight || panningLeft)
  {
    var t = percent_change_chart.transition().duration(750);

    if(panningLeft)
    {
      console.log("Panning Left");
      if(!(beginningIndex - 1 <= 0))
      {
        beginningIndex -= 1;
        endingIndex -= 1;
      }
      else
      {
        return;
      }
    }
    else if(panningRight)
    {
      console.log("Panning Right");
      if(!(endingIndex + 1 >= DailyPercentageChangesWithDates.length - 1))
      {
        beginningIndex += 1;
        endingIndex += 1;
      }
      else
      {
        return;
      }
    }

    renderPercentChangeArray = [];

    clearChart("account_percentage_change_line_chart");
    percent_change_chart = d3.select(".account_percentage_change_line_chart")
    .attr("width", outerWidth)
    .attr("height", outerHeight)
    .append("g")
    .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

    renderPercentChangeChart();
  } 
}

function renderPercentChangeChart()
{
//Percentage Change Line Graph
    percent_line_x_domain = d3.extent(DailyPercentageChangesWithDates, function(d, i) { return d.date; });
    percent_line_y_domain = [d3.min(DailyPercentageChangesWithDates, function(d, i) { return d.change; }), d3.max(DailyPercentageChangesWithDates, function(d, i) { return d.change; })];

    percent_line_domain_beginning = DailyPercentageChangesWithDates[beginningIndex].date;
    percent_line_domain_end = DailyPercentageChangesWithDates[endingIndex].date;

    renderPercentChangeArray = DailyPercentageChangesWithDates.slice(beginningIndex, endingIndex);

    //percent_line_x.domain(percent_line_x_domain);
    percent_line_x.domain([percent_line_domain_beginning, percent_line_domain_end]);
    percent_line_y.domain(percent_line_y_domain);

    var percent_g = percent_change_chart.append("g")
      .attr("transform", "translate(" + padding.left + "," + padding.top + ")");

  //percent_change_chart.transition().duration(750);

        // Add the percentline path.
  percent_g.attr("class", "charts")
      .append("path")
      //.data([DailyPercentageChangesWithDates])
      .data([renderPercentChangeArray])
      .attr("class", "line")
      .attr("d", percentline)
      .attr("fill", "none")
      .attr("stroke", "blue")
      .attr("stroke-width", 1.5);

  percent_line_xAxis = d3.axisBottom(percent_line_x);

    // Add the x Axis
  percent_g.append("g")
    .attr("class", "axis axis--x")
    .attr("transform", "translate(0," + height + ")")
    .call(percent_line_xAxis);

    // text label for the x axis
  percent_g.append("text")             
      .attr("transform",
            "translate(" + ((width/2) + margin.left) + " ," + 
                           (height + (margin.bottom) + margin.top) + ")")
      .style("text-anchor", "middle")
      .text("Date");

  percent_line_yAxis = d3.axisLeft(percent_line_y);

    // Add the y Axis
  percent_g.append("g")
    .attr("class", "axis axis--y")
    .attr("transform", "translate(" + margin.left + ",0)")
    .call(percent_line_yAxis);

  // text label for the y axis
  percent_change_chart.append("text")
      .attr("transform", "rotate(-90)")
      .attr("y", 0 + (margin.left/4))
      .attr("x",0 - (((height)/1.8) + margin.top + margin.bottom) )
      .attr("dy", "1em")
      .style("text-anchor", "middle")
      .text("Percent");

  percent_change_chart.append("text")
      .attr("x", 0 + (outerWidth/2))
      .attr("y", 0)
      .style("text-anchor", "middle")
      .text("Account Percentage Change By Day");
}

function renderSharesOfStocksChart()
{
// Scale the range of the data
var shares_x = d3.scaleBand()
    .range([margin.left, width]);

  // var shares_x = d3.scaleOrdinal()
  //   .range([margin.left, width]);

  var shares_x_axis = shares_x.domain(RenderedSharesOfStocks.map(function(d) { return d.stock; }));

  var shares_y = d3.scaleLinear()
    .range([height - margin.bottom, margin.top]);

  var max = d3.max(RenderedSharesOfStocks, function(d) { return d.amount; });

  var shares_y_axis = shares_y.domain([0, max]);

  var tip = d3.tip()
  .attr('class', 'd3-tip')
  .offset([-10, 0])
  .html(function(d) {
    return "<center>" + d.stock + "</center>" + "<strong>Shares:</strong> <span style='color:magenta'>" + d.amount + "</span><br>" ;
  })

  shares_of_stocks_bought_chart.call(tip);

  // Add the x Axis
  shares_of_stocks_bought_chart.append("g")
      .attr("transform", "translate(" + (margin.left + (margin.right/2)) + ", " + (height - margin.top) + ")")
      .call(d3.axisBottom(shares_x))
      .selectAll("text")  
        .style("text-anchor", "end")
        .attr("dx", "-.8em")
        .attr("dy", "-.15em")
        .attr("transform", "rotate(-45)");

    // text label for the x axis
  shares_of_stocks_bought_chart.append("text")             
      .attr("transform",
            "translate(" + ((width/2) + margin.left + (margin.right)) + " ," + 
                           (height + 35) + ")")
      .style("text-anchor", "middle")
      .text("Stock");

  // Add the y Axis
  shares_of_stocks_bought_chart.append("g")
      .attr("transform", "translate(" + (margin.left + margin.right + (margin.left / 2)) + ", " + 0 + ")")
      .call(d3.axisLeft(shares_y));

  // text label for the y axis
  shares_of_stocks_bought_chart.append("text")
      .attr("transform", "rotate(-90)")
      .attr("y", 0)
      .attr("x",0 - (height/2))
      .attr("dy", "1em")
      .style("text-anchor", "middle")
      .text("Amount");

  // Add the valueBar bar
   shares_of_stocks_bought_chart.selectAll(".shares_bar")
      .data(RenderedSharesOfStocks)
      .enter().append("rect")
      .attr("class", "shares_bar")
      .attr("x", d => shares_x_axis(d.stock)  + (margin.left + (margin.right/2)))
      .attr("y", function(d) { return shares_y_axis(d.amount); })
      .attr("height", function(d) { return height - shares_y_axis(d.amount) - margin.top; })
      .attr("width", shares_x.bandwidth())
      .on('mouseover', tip.show)
      .on('mouseout', tip.hide);

    shares_of_stocks_bought_chart.append("text")
      .attr("x", 0 + (outerWidth/2))
      .attr("y", 0)
      .style("text-anchor", "middle")
      .text("Top Most Purchased Stocks");
}

function RenderTextData()
{ 
  DPSText.append("text")
        .attr("x", 50)             
        .attr("y", (text_height/2) + 5)
        //.attr("text-anchor", "middle") 
        .style("font-size", "16px")
        .text("Damage Per Second: " + d3.format(".2f")(DPS));

  DamageReceivedText.append("text")
        .attr("x", 50)             
        .attr("y", (text_height/2) + 5)
        //.attr("text-anchor", "middle")  
        .style("font-size", "16px")
        .text("Total Damage Received: " + d3.format("$,.2f")((DamageReceived)));

  AverageFrameTimeText.append("text")
        .attr("x", 45)             
        .attr("y", (text_height/2) + 5)
        //.attr("text-anchor", "middle")  
        .style("font-size", "16px")
        .text("Average Frame Time: " + d3.format(".2%")(AverageFrameTime));
}

function colorOfBar(percent)
{
  //green
  if(percent >= 0)
  {
    return "#00b300";
  }
  //red
  else
  {
    return "#b30000";
  }
}

function colorOfShares(amount, max)
{
  //green
  if(amount <= max / 3)
  {
    return "#ffff00";
  }
  //red
  else if(amount <= (max * (2/3)))
  {
    return "#ff8000";
  }
  else
  {
    return "#80ff00";
  }
}

function AddToSharesOfStockArray(purchase)
{
  var added = false;
  //Add back to shares
  for(var i = 0; i < SharesOfStocks.length; ++i)
  {
    if(purchase.stock == SharesOfStocks[i].stock)
    {
      SharesOfStocks[i].amount += purchase.amount;
      added = true;
    }
  }
  
  if(!added)
  {
    SharesOfStocks.push({stock: purchase.stock, amount: purchase.amount});
  }
}

function ClampSharesOfStockArray()
{
  if(SharesOfStocks.length > maxSharesShown)
  {
    //Sort based on amount
    SharesOfStocks.sort(function(a, b){return b.amount - a.amount});

    for(var i = 0; i < maxSharesShown; ++i)
    {
      RenderedSharesOfStocks.push(SharesOfStocks[i]);
    }

    //Sort by alphebetically
    RenderedSharesOfStocks.sort(function(a, b)
    {
      var x = a.stock.toLowerCase();
      var y = b.stock.toLowerCase();
      if (x < y) {return -1;}
      if (x > y) {return 1;}
      return 0;
    });
  }
  else
  {
    //Sort based on amount
    SharesOfStocks.sort(function(a, b){return b.amount - a.amount});
    RenderedSharesOfStocks = SharesOfStocks;
    RenderedSharesOfStocks.sort(function(a, b)
    {
      var x = a.stock.toLowerCase();
      var y = b.stock.toLowerCase();
      if (x < y) {return -1;}
      if (x > y) {return 1;}
      return 0;
    });
  }
}

function error(result){
    console.log("Couldn't load CSV");
    console.log("Result: " + result);
}