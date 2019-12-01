
// jQuery("#loading").toggle();
Promise.all([
  d3.csv("https://raw.githubusercontent.com/PvtBooth/cs399project6/master/project7/UnitTests/BOMB.csv"),
  d3.csv("https://raw.githubusercontent.com/PvtBooth/cs399project6/master/project7/UnitTests/GRAVITAS.csv"),
  d3.csv("https://raw.githubusercontent.com/PvtBooth/cs399project6/master/project7/UnitTests/MISSILE.csv"),
  d3.csv("https://raw.githubusercontent.com/PvtBooth/cs399project6/master/project7/UnitTests/SPRAY.csv"),
  d3.csv("https://raw.githubusercontent.com/PvtBooth/cs399project6/master/project7/UnitTests/LEVEL1.csv"),
  d3.csv("https://raw.githubusercontent.com/PvtBooth/cs399project6/master/project7/UnitTests/LEVEL7.csv"),
  d3.csv("https://raw.githubusercontent.com/PvtBooth/cs399project6/master/project7/UnitTests/LEVEL15.csv"),
  d3.csv("https://raw.githubusercontent.com/PvtBooth/cs399project6/master/project7/UnitTests/LEVEL50.csv"),
  d3.csv("https://raw.githubusercontent.com/PvtBooth/cs399project6/master/project7/UnitTests/LEVEL100.csv"),
])
.then(function(data) {   
  main(data);
})
.catch(function(result) {
  error(result);
});

var bombData, gravitasData, missileData, sprayData, l1Data, l2Data, l3Data, l4Data, l5Data;
function main(data)
{
  jQuery("#loading").toggle();
  bombData = data[0];
  gravitasData = data[1];  
  missileData = data[2];
  sprayData = data[3];
  l1Data = data[4];
  l2Data = data[5];
  l3Data = data[6];
  l4Data = data[7];
  l5Data = data[8];
  AnalyzeUnitTest(bombData);
  RenderDamageChart();
  RenderHeatMap();
  RenderTextData();
  AnalyzeSystemTest(l1Data);
  RenderAverageTimes();
}

// Main Data
var playerHealth;
var enemyHealth;
var numEnemies;

var strategy = "s0";
var DPS = 0.0;
var DamageReceived = 0.0;
var enemies = [];
var initialDamageTime = -1.0;

// Per frame stored values
var PerFrameData = []; // ENTRIES: Time, PlayerHP, EnemiesHP, TotalEnemies
var PlayerPosition = [];
function AnalyzeUnitTest(data)
{
  PerFrameData = [];
  PlayerPosition = [];
  enemies = [];
  playerHealth = 100.0;
  enemyHealth = 0.0;
  numEnemies = 0;
  initialDamageTime = -1.0;

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
  else if(eventLog.LogType == "L_transform")
  {
    PlayerPosition.push({
      Time: parseFloat(eventLog.Time),
      X: parseFloat(eventLog.Team),
      Y: parseFloat(eventLog.Weapon),
      Fill: 50
    });
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

  var lastInfo =  PerFrameData[PerFrameData.length - 1];
  var FightDuration = lastInfo.Time - initialDamageTime;
  DPS = 120 / FightDuration;
  DamageReceived = 100 - lastInfo.PlayerHP;
  AverageFrameTime = lastInfo.Time / PerFrameData.length;
}

function GetEnemyTotalHP(){
  var HP = 0.0;
  enemies.forEach(enemy => HP += enemy.HP > 0.0 ? enemy.HP : 0.0);
  return HP;
}

var SystemData = [];
function AnalyzeSystemTest(data)
{
  SystemData = [];
  SystemData.push({System: "AE_Graphics", TimeTotal: 0.0, Count : 0, AverageTime : 0.0, Min: 10.0, Max: 0.0, StdDev: 0.0});
  SystemData.push({System: "Physics", TimeTotal: 0.0, Count : 0, AverageTime : 0.0, Min: 10.0, Max: 0.0, StdDev: 0.0});
  SystemData.push({System: "Entites", TimeTotal: 0.0, Count : 0, AverageTime : 0.0, Min: 10.0, Max: 0.0, StdDev: 0.0});
  SystemData.push({System: "AI", TimeTotal: 0.0, Count : 0, AverageTime : 0.0, Min: 10.0, Max: 0.0, StdDev: 0.0});
  SystemData.push({System: "Input", TimeTotal: 0.0, Count : 0, AverageTime : 0.0, Min: 10.0, Max: 0.0, StdDev: 0.0});
  SystemData.push({System: "Sound", TimeTotal: 0.0, Count : 0, AverageTime : 0.0, Min: 10.0, Max: 0.0, StdDev: 0.0});

  data.forEach(function(eventLog) 
  {
    if(eventLog.LogType === "L_system_time")
    {
      SystemData.forEach(sys => 
      {
        if(sys.System === eventLog.SourceType)
        {
          var time = parseFloat(eventLog.SourceID);
          sys.TimeTotal += time;
          sys.Count++;
          if(time > sys.Max)
            sys.Max = time;
          if(time < sys.Min)
            sys.Min = time;
        }
      });
    }
  })
  SystemData.forEach(sys => 
  {
    var rms = 0.0;
    if(sys.Count != 0)
      sys.AverageTime = sys.TimeTotal / sys.Count;
    data.forEach(function(eventLog) 
    {
      if(eventLog.LogType === "L_system_time" && sys.System === eventLog.SourceType)
      {
        var delta = parseFloat(eventLog.SourceID) - sys.AverageTime;
        rms += delta*delta;
      }
    });
    if(sys.Count != 0)
      sys.StdDev = Math.sqrt(rms / sys.Count);
  });
}

var SystemName = "";
var PerLevelSystemData = [];
function OnBarClick(data,i)
{
  //System Time Bar Chart
  clearChart("SystemAverageLevelChart");
  SystemAverageLevelChart = d3.select(".SystemAverageLevelChart")
    .attr("width", outerWidth)
    .attr("height", outerHeight)
    .append("g")
    .attr("transform", "translate(" + margin.left + "," +margin.top + ")");

  SystemName = data.System;
  
  PerLevelSystemData = [];
  AnalyzeSystemTest(l1Data); PerLevelSystemData.push({Level: "Level 1", AverageTime : GetSystemAverageTime(SystemName)});
  AnalyzeSystemTest(l2Data); PerLevelSystemData.push({Level: "Level 7", AverageTime : GetSystemAverageTime(SystemName)});
  AnalyzeSystemTest(l3Data); PerLevelSystemData.push({Level: "Level 15", AverageTime : GetSystemAverageTime(SystemName)});
  AnalyzeSystemTest(l4Data); PerLevelSystemData.push({Level: "Level 50", AverageTime : GetSystemAverageTime(SystemName)});
  AnalyzeSystemTest(l5Data); PerLevelSystemData.push({Level: "Level 100", AverageTime : GetSystemAverageTime(SystemName)});
  RenderSystemAverageLevelChart();
}

function GetSystemAverageTime(name)
{
  var time = 0.0;
  SystemData.forEach(sys => 
  {
    if(sys.System === name)
      time = sys.AverageTime;
  })
  return time;
}

var margin = {top: 20, right: 20, bottom: 20, left: 20},
    padding = {top: 60, right: 60, bottom: 60, left: 60},
    outerWidth = 910,
    outerHeight = 400,
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

function clearChart(chartName)
{
  $("#" + chartName).empty();
}

var changeStrategy = function(p_strategy)
{
  //Damage Chart
  clearChart("DamageChart");
  DamageChart = d3.select(".DamageChart")
    .attr("width", outerWidth)
    .attr("height", outerHeight)
    .append("g")
    .attr("transform", "translate(" + margin.left + "," + margin.top + ")");
  
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
  RenderDamageChart();
  RenderTextData();
  RenderHeatMap();
}

var changeLevel = function(p_level)
{
  //System Time Bar Chart
  clearChart("SystemAverageTimeChart");
  SystemAverageTimeChart = d3.select(".SystemAverageTimeChart")
    .attr("width", outerWidth)
    .attr("height", outerHeight)
    .append("g")
    .attr("transform", "translate(" + margin.left + "," +margin.top + ")");

  if(p_level == "s0")
    AnalyzeSystemTest(l1Data);
  else if(p_level == "s1")
    AnalyzeSystemTest(l2Data);
  else if(p_level == "s2")
    AnalyzeSystemTest(l3Data);
  else if(p_level == "s3")
    AnalyzeSystemTest(l4Data);
  else if(p_level == "s4")
    AnalyzeSystemTest(l5Data);
  
    RenderAverageTimes();
}

// ============================ SYSTEM_HISTOGRAM ====================================
var SystemAverageTimeChart = d3.select(".SystemAverageTimeChart")
    .attr("width", outerWidth)
    .attr("height", outerHeight)
    .append("g")
    .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

function RenderAverageTimes()
{
  SystemAverageTimeChart.append("text")
    .attr("x", 0 + (outerWidth/2))
    .attr("y", 0)
    .style("text-anchor", "middle")
    .text("Sytem Frame Information");

  // Scale the range of the data
  var xScale = d3.scaleBand().range([margin.left, width]);
  var yScale = d3.scaleLinear().range([height, margin.top]);

  var xDomain = xScale.domain(SystemData.map(function(d) { return d.System; }));
  var max = d3.max(SystemData, function(d) { return d.AverageTime; });
  var yDomain = yScale.domain([0, max]);

  var tip = d3.tip()
  .attr('class', 'd3-tip')
  .offset([-10, 0])
  .html(function(d) {
    return "<center>" + d.System + "</center>" + 
    "<strong>AverageTime: </strong> <span style='color:magenta'>" + d.AverageTime + "</span><br>" + 
    "<strong>Standard Deviation: </strong> <span style='color:magenta'>" + d.StdDev + "</span><br>" + 
    "<strong>Min: </strong> <span style='color:magenta'>" + d.Min + "</span><br>" + 
    "<strong>Max: </strong> <span style='color:magenta'>" + d.Max + "</span><br>" + 
    "<strong>Count: </strong> <span style='color:magenta'>" + d.Count + "</span><br>";
  })

  SystemAverageTimeChart.call(tip);

  SystemAverageTimeChart_g = SystemAverageTimeChart.append("g")
    .attr("transform", "translate(" + padding.left + "," + padding.top + ")");

  SystemAverageTimeChart_g.append("g")
    .attr("transform", "translate(0," + height + ")")
    .call(d3.axisBottom(xScale))
    .selectAll("text")  
      .style("text-anchor", "end")
      .attr("dx", "-.8em")
      .attr("dy", "-.15em")
      .attr("transform", "rotate(-45)");
  
  // text label for the x axis
  SystemAverageTimeChart_g.append("text")             
    .attr("transform",
    "translate(" + ((width/2) + margin.left) + " ," + 
                    (height + (margin.bottom) + margin.top) + ")")
      .style("text-anchor", "middle")
      .text("System");

  // Add the y Axis
  SystemAverageTimeChart_g.append("g")
    .attr("transform", "translate(" + margin.left + ",0)")
    .call(d3.axisLeft(yScale));

  // text label for the y axis
  SystemAverageTimeChart.append("text")
    .attr("transform", "rotate(-90)")
    .attr("y", 0 + (margin.left/4))
    .attr("x",0 - (((height)/1.8) + margin.top + margin.bottom) )
    .attr("dy", "1em")
    .style("text-anchor", "middle")
    .text("Average Time per Frame");
    
  // Add the valueBar bar
  SystemAverageTimeChart_g.selectAll(".shares_bar")
      .data(SystemData)
      .enter().append("rect")
      .attr("class", "shares_bar")
      .attr("x", d => xDomain(d.System))
      .attr("y", function(d) { return yDomain(d.AverageTime); })
      .attr("height", function(d) { return height - yDomain(d.AverageTime) })
      .attr("width", xScale.bandwidth())
      .on('mouseover', tip.show)
      .on('mouseout', tip.hide)
      .on('click', (d,i) => OnBarClick(d,i));
}

// ============================= SYSTEM_LEVEL_CHART ==============================

var SystemAverageLevelChart = d3.select(".SystemAverageLevelChart")
    .attr("width", outerWidth)
    .attr("height", outerHeight)
    .append("g")
    .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

function RenderSystemAverageLevelChart()
{
  clearChart("SystemAverageLevelChart");
  SystemAverageLevelChart = d3.select(".SystemAverageLevelChart")
    .attr("width", outerWidth)
    .attr("height", outerHeight)
    .append("g")
    .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

  // Scale the range of the data
  var xScale = d3.scaleBand().range([margin.left, width]);
  var yScale = d3.scaleLinear().range([height, margin.top]);

  var xDomain = xScale.domain(PerLevelSystemData.map(function(d) { return d.Level; }));
  var max = d3.max(PerLevelSystemData, function(d) { return d.AverageTime; });
  var yDomain = yScale.domain([0, max]);
  var SystemLine = d3.line()
    .curve(d3.curveCardinal)
    .x(function(d) { return xScale(d.Level); })
    .y(function(d) { return yScale(d.AverageTime); });

  var SystemAverageLevelChart_g = SystemAverageLevelChart.append("g")
    .attr("transform", "translate(" + padding.left + "," + padding.top + ")");

  // Add the valueline path.
  SystemAverageLevelChart_g.append("path")
      .data([PerLevelSystemData])
      .attr("class", "line")
      .attr("d", SystemLine)
      .attr("fill", "none")
      .attr("stroke", "Grey")
      .attr("stroke-width", 1.5);

  // Add the x Axis
  SystemAverageLevelChart_g.append("g")
    .attr("class", "x axis")
    .attr("transform", "translate(0," + height + ")")
    .call(d3.axisBottom(xScale));

  // text label for the x axis
  SystemAverageLevelChart_g.append("text")             
      .attr("transform",
            "translate(" + ((width/2) + margin.left) + " ," + 
                           (height + (margin.bottom) + margin.top) + ")")
      .style("text-anchor", "middle")
      .text("Level");

  // Add the y Axis
  SystemAverageLevelChart_g.append("g")
    .attr("class", "y axis")
    .attr("transform", "translate(" + margin.left + ",0)")
    .call(d3.axisLeft(yScale));

  // text label for the y axis
  SystemAverageLevelChart.append("text")
      .attr("transform", "rotate(-90)")
      .attr("y", 0 + (margin.left/4))
      .attr("x",0 - (((height)/1.8) + margin.top + margin.bottom) )
      .attr("dy", "1em")
      .style("text-anchor", "middle")
      .text("Average Time per Frame");

  SystemAverageLevelChart.append("text")
      .attr("x", 0 + (outerWidth/2))
      .attr("y", 0)
      .style("text-anchor", "middle")
      .text("Per Level Performance Analysis");
}


// ============================ DAMAGE_CHART ====================================
var DamageChart = d3.select(".DamageChart")
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

var tooltipLine;
var tooltip = d3.tip()  
  .attr('class', 'd3-tip')
  .offset([-10, 0]);
var tipBox;
function RenderDamageChart()
{
    // Damage Graph:x Time,y1 PlayerHP,y2 EnemiesHP,y3 TotalEnemies
    // Scale the range of the data
  xScaleDamage.domain(d3.extent(PerFrameData, function(d, i) { return d.Time; }));
  yScaleDamage.domain([0, d3.max(PerFrameData, function(d, i) { return d.EnemiesHP; })]);

  var DamageChart_g = DamageChart.append("g")
    .attr("transform", "translate(" + padding.left + "," + padding.top + ")");
  
  tooltipLine = DamageChart_g.append('line');
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

  DamageChart_g.call(tooltip);
  
  tipBox = DamageChart_g.append('rect')
      .attr('width', width)
      .attr('height', height)
      .attr('opacity', 0)
      .on('mouseover', tooltip.show)
      .on('mousemove', drawTooltip)
      .on('mouseout', removeTooltip);
}

function removeTooltip() {
  if (tooltipLine) tooltipLine.attr('stroke', 'none');
  ResetPlayerPositionFill(50);
  RenderHeatMap();
  tooltip.hide();
}
function drawTooltip() {
  const time = xScaleDamage.invert(d3.mouse(tipBox.node())[0]);    
  tooltipLine.attr('stroke', 'black')
    .attr('x1', xScaleDamage(time))
    .attr('x2', xScaleDamage(time))
    .attr('y1', 0)
    .attr('y2', height);

  tooltip.hide();
  var log = PerFrameData.find(element => element.Time > time);
  tooltip.offset([0, xScaleDamage(time) - width/2])
    .html("<strong>Frame Time: </strong> <span style='color:white'>" + log.Time + "</span><br>" +
    "<strong>Player HP: </strong> <span style='color:green'>" + log.PlayerHP + "</span><br>" + 
    "<strong>Enemy Total HP: </strong> <span style='color:orange'>" + log.EnemiesHP + "</span><br>" +
    "<strong>Enemy Count: </strong> <span style='color:red'>" + log.TotalEnemies + "</span><br>");

  ResetPlayerPositionFill(30);
  PlayerPosition.find(entry => entry.Time - initialDamageTime >  log.Time).Fill = 100;
  RenderHeatMap();
  tooltip.show();
}

function ResetPlayerPositionFill(fillVal)
{
  PlayerPosition.forEach(d => d.Fill = fillVal)
}

// ============================ Heat Map ====================================

var HeatMap = d3.select(".HeatMap")
    .attr("width", outerWidth)
    .attr("height", outerHeight)
    .append("g")
    .attr("transform", "translate(" + margin.left + "," + margin.top + ")");
    
function RenderHeatMap()
{
  clearChart("HeatMap");
  HeatMap = d3.select(".HeatMap")
  .attr("width", outerWidth)
  .attr("height", outerHeight)
  .append("g")
  .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

  HeatMap.append("text")
  .attr("x", 0 + (outerWidth/2))
  .attr("y", 0)
  .style("text-anchor", "middle")
  .text("Player Heat Map");
  // Scale the range of the data
  var xScale = d3.scaleLinear().range([margin.left, height]);
  var yScale = d3.scaleLinear().range([height, margin.top]);
  var xMax = d3.max(PlayerPosition, function(d) { return d.X; });
  var xMin = d3.min(PlayerPosition, function(d) { return d.X; });
  var yMax = d3.max(PlayerPosition, function(d) { return d.Y; });
  var yMin = d3.min(PlayerPosition, function(d) { return d.Y; });
  var xDomain, yDomain;
  
  var deltaRange = xMax - xMin - (yMax - yMin);
  if(deltaRange > 0)
  {
    xDomain = xScale.domain([xMin, xMax]);  
    yDomain = yScale.domain([yMin - deltaRange / 2, yMax + deltaRange / 2]);
  }
  else
  {
    xDomain = xScale.domain([xMin + deltaRange / 2, xMax - deltaRange / 2]);  
    yDomain = yScale.domain([yMin, yMax]);
  }
   
  var HeatMap_g = HeatMap.append("g")
    .attr("transform", "translate(" + (padding.left + (width - height) / 2) + "," + padding.top + ")");
  
    HeatMap_g.append("g")
    .attr("transform", "translate(0," + height + ")")
    .call(d3.axisBottom(xScale))
    .selectAll("text")  
      .style("text-anchor", "end")
      .attr("dx", "-.8em")
      .attr("dy", "-.15em")
      .attr("transform", "rotate(-45)");
  
  // text label for the x axis
  HeatMap_g.append("text")             
    .attr("transform",
    "translate(" + (((width/2) + margin.left) - (width - height) / 2) + " ," + 
                    (height + (margin.bottom) + margin.top + 10) + ")")
      .style("text-anchor", "middle")
      .text("X");

  // Add the y Axis
  HeatMap_g.append("g")
    .attr("transform", "translate(" + margin.left + ",0)")
    .call(d3.axisLeft(yScale));

  // text label for the y axis
  HeatMap.append("text")
    .attr("x", 0 + (margin.left/4) + (width - height) / 2)
    .attr("y", (((height)/1.8) + margin.top + margin.bottom) )
    .attr("dy", "1em")
    .style("text-anchor", "middle")
    .text("Y");

  // Build color scale
  var myColor = d3.scaleLinear()
    .range(["white", "red"])
    .domain([1,100])
    
  // Add the valueBar bar
  HeatMap_g.selectAll()
      .data(PlayerPosition)
      .enter()
      .append("rect")
      .attr("x", d => xDomain(d.X))
      .attr("y", d => yDomain(d.Y))
      .attr("width", 2.0 )
      .attr("height", 2.0 )
      .style("fill", d => myColor(d.Fill) )
}

function RenderTextData()
{ 
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
  AverageFrameTimeText = d3.select(".AverageFrameTimeText")
    .attr("width", text_outerWidth)
    .attr("height", text_outerHeight)
    .append("g")
    .attr("transform", "translate(" + text_margin.left + "," + text_margin.top + ")");

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
        .text("Total Damage Received: " + d3.format(".2f")((DamageReceived)));

  AverageFrameTimeText.append("text")
        .attr("x", 45)             
        .attr("y", (text_height/2) + 5)
        //.attr("text-anchor", "middle")  
        .style("font-size", "16px")
        .text("Average Frame Time: " + d3.format(".6f")(AverageFrameTime));
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

function error(result){
    console.log("Couldn't load CSV");
    console.log("Result: " + result);
}