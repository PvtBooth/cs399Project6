//Remove loading text
jQuery("#loading").toggle();

var goodToRun = true;

var startSimulation = function()
{
  if(goodToRun)
  {
    jQuery("#loading").toggle();
    d3.csv("https://raw.githubusercontent.com/PvtBooth/cs399project6/master/StockDataGenerator/DATA.csv").then(main, error);
  }
  else
  {
    alert ("Start Date is after End Date");
  }
}

// Main Data
var dates;
var stocks;
var prices;

// Simulation Data
var startDate = "2009-01-02";
var endDate = "2019-10-11";

var strategy = "s0";
var startingMoney = 100000;
var currentMoney = startingMoney;
var stockValue = 0;
var current_date; //String 
var purchases = []; // Array of current purchases

//Need to calculate
var AccountPercentageGain = 0.0;
var AverageDailyPercentageGain = 0.0;
var DailyStandardDeviation = 0.0;
var YearlyPercentageGain = 0.0;
var YearlyStandardDeviation = 0.0;
var SharpeRatio = 0.0;
var SPYPercentageGain = 0.0;
var MaxDrawdownPercentage = 0.0;

//Graph vars
var DailyAccountValue = [];
var DailyPercentageChanges = [];
var DailyPercentageChangesWithDates = [];
var MoneyChangePerYear = [];
var MoneyChangePerMonth = [];
var SharesOfStocks = [];

var old_margin = {top: 20, right: 30, bottom: 30, left: 80},
    old_width = 1250 - old_margin.left - old_margin.right,
    old_height = 550 - old_margin.top - old_margin.bottom;

var margin = {top: 20, right: 20, bottom: 20, left: 20},
    padding = {top: 60, right: 60, bottom: 60, left: 60},
    outerWidth = 960,
    outerHeight = 500,
    innerWidth = outerWidth - margin.left - margin.right,
    innerHeight = outerHeight - margin.top - margin.bottom,
    width = innerWidth - padding.left - padding.right,
    height = innerHeight - padding.top - padding.bottom;

// set the ranges
var money_line_x = d3.scaleTime().range([margin.left, width]);
var money_line_y = d3.scaleLinear().range([height, (0)]);

var percent_line_x = d3.scaleTime().range([margin.left, width]);
var percent_line_y = d3.scaleLinear().range([height, (0)]);

// define the line
var moneyline = d3.line()
    .x(function(d) { return money_line_x(d.date); })
    .y(function(d) { return money_line_y(d.money); });

var percentline = d3.line()
    .x(function(d) { return percent_line_x(d.date); })
    .y(function(d) { return percent_line_y(d.change); });

var percent_histogram_chart;
var money_chart;
var percent_change_chart;
var shares_of_stocks_bought_chart;
var text_object;

var histogram_left_max = -8.0;
var histogram_right_max = 8.0;

var ResetVolatileData = function()
{
  startingMoney = 100000;
  currentMoney = startingMoney;
  stockValue = 0;
  purchases = [];

  AccountPercentageGain = 0.0;
  AverageDailyPercentageGain = 0.0;
  DailyStandardDeviation = 0.0;
  YearlyPercentageGain = 0.0;
  YearlyStandardDeviation = 0.0;
  SharpeRatio = 0.0;
  SPYPercentageGain = 0.0;
  MaxDrawdownPercentage = 0.0;

  DailyAccountValue = [];
  DailyPercentageChanges = [];
  DailyPercentageChangesWithDates = [];
  MoneyChangePerYear = [];
  MoneyChangePerMonth = [];
  SharesOfStocks = [];

  d3.select("body").selectAll("svg").remove();

  percent_histogram_chart = d3.select("body").append("svg")
    .attr("width", outerWidth)
    .attr("height", outerHeight)
    .append("g")
    .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

  money_chart = d3.select("body").append("svg")
    .attr("width", outerWidth)
    .attr("height", outerHeight)
    .append("g")
    .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

  percent_change_chart = d3.select("body").append("svg")
    .attr("width", outerWidth)
    .attr("height", outerHeight)
    .append("g")
    .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

  shares_of_stocks_bought_chart = d3.select("body").append("svg")
    .attr("width", outerWidth)
    .attr("height", outerHeight)
    .append("g")
    .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

  text_object = d3.select("body").append("svg")
    .attr("width", outerWidth)
    .attr("height", outerHeight)
    .append("g")
    .attr("transform", "translate(" + margin.left + "," + margin.top + ")");
}

var changeStrategy = function(p_strategy)
{
  strategy = p_strategy;
}

var changeStartDate = function(p_date)
{
  var start_date = p_date.value;
  var end_calendar = document.getElementById('end');

  var d1 = Date.parse(start_date);
  var d2 = Date.parse(end_calendar.value);
  
  if (d1 > d2)
  {
    //confirm ("Start Date is after End Date");
    goodToRun = false;
  }
  else
  {
    goodToRun = true;

    startDate = start_date;
    endDate = end_calendar.value;
  }
}

var changeEndDate = function(p_date)
{
  var end_date = p_date.value;
  var start_calendar = document.getElementById('end');

  var d1 = Date.parse(endDate);
  var d2 = Date.parse(start_calendar.value);
  
  if (d1 < d2)
  {
    //confirm ("Start Date is after End Date");
    goodToRun = false;
  }
  else
  {
    goodToRun = true;

    startDate = start_calendar.value;
    endDate = end_date;
  }
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

function main(data)
{
    //Reset any data that needs to be cleared before a strategy is run.
    ResetVolatileData();
    // Get all data 
    stocks = Object.keys(data[0]);
    stocks.shift();
    dates = data.map(d => d.Date);
    prices = data.reduce((map, entry) => 
    {
        map[entry.Date] = entry;
        return map;
    }, {});
   
      // Run through dates and perform a certain strategy over start to end dates
    var dateRange = dates.slice(dates.indexOf(startDate), dates.indexOf(endDate));
    DailyAccountValue.push({ date: new Date(dates[0]), money: currentMoney + stockValue });
    dateRange.forEach(function(date)
    {
        // console.log(date);
        if(strategy == "s0")
        {
          RandomStrategy(date, 0.01); // almost 5 stocks per day
        }
        else if(strategy == "s1")
        {

        }
        else if(strategy == "s2")
        {
          
        }


        stockValue = GetTotalStockValue(date);
        CalculatePercentageGain();
        CheckDrawdown();
        
        DailyAccountValue.push({ date: new Date(date), money: currentMoney + stockValue});
        // console.log("Stocks Value: " + stockValue);
        // console.log("Cash Value: " + currentMoney);
        // console.log("Percentage Gain: " + AccountPercentageGain);
        // console.log("");
    });
    DailyAccountValue.shift();

    CalculateMoneyChangePerYear();

    AccountPercentageGain = (currentMoney + stockValue - startingMoney)* 100 / startingMoney;
    var avg = 0;
    DailyPercentageChanges.forEach(d => { avg += d; });
    AverageDailyPercentageGain = avg / DailyPercentageChanges.length;

    var variance = 0;
    DailyPercentageChanges.forEach(d => { variance += Math.pow(d - AverageDailyPercentageGain, 2)});
    DailyStandardDeviation = Math.sqrt(variance / DailyPercentageChanges.length);

    YearlyPercentageGain = (Math.pow(1+(AverageDailyPercentageGain/100), 252) - 1) * 100;
    YearlyStandardDeviation = DailyStandardDeviation*Math.sqrt(252);
    SharpeRatio = ((YearlyPercentageGain) - 0.035) / YearlyStandardDeviation;
    SPYPercentageGain = (GetPrice(endDate, " SPY") - GetPrice(startDate, " SPY")) * 100 / GetPrice(startDate, " SPY");
    

    //Remove loading text
    jQuery("#loading").toggle();

    //Money Graph
    // Scale the range of the data
    money_line_x.domain(d3.extent(DailyAccountValue, function(d, i) { return d.date; }));
    money_line_y.domain([0, d3.max(DailyAccountValue, function(d, i) { return d.money; })]);

  var money_g = money_chart.append("g")
    .attr("transform", "translate(" + padding.left + "," + padding.top + ")");

        // Add the valueline path.
  money_g.append("path")
      .data([DailyAccountValue])
      .attr("class", "line")
      .attr("d", moneyline)
      .attr("fill", "none")
      .attr("stroke", "green")
      .attr("stroke-width", 1.5);

    // Add the x Axis
  money_g.append("g")
    .attr("class", "x axis")
    .attr("transform", "translate(0," + height + ")")
    .call(d3.axisBottom(money_line_x));

    // text label for the x axis
  money_g.append("text")             
      .attr("transform",
            "translate(" + ((width/2) + margin.left) + " ," + 
                           (height + (margin.bottom) + margin.top) + ")")
      .style("text-anchor", "middle")
      .text("Date");


    // Add the y Axis
  money_g.append("g")
    .attr("class", "y axis")
    .attr("transform", "translate(" + margin.left + ",0)")
    .call(d3.axisLeft(money_line_y));

  // text label for the y axis
  money_chart.append("text")
      .attr("transform", "rotate(-90)")
      .attr("y", 0 + (margin.left/4))
      .attr("x",0 - (((height)/1.8) + margin.top + margin.bottom) )
      .attr("dy", "1em")
      .style("text-anchor", "middle")
      .text("Money");


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
    .range([height - margin.bottom, 0]);

var bar_y_axis = bar_y.domain([0, d3.max(bins, function(d) { return d.length; })]);

 // Add the x Axis
  percent_histogram_chart.append("g")
      .attr("transform", "translate(" + (margin.left + (margin.right/2)) + ", " + (height - margin.top) + ")")
      .call(d3.axisBottom(bar_x).ticks(histogram_right_max * 4));

    // text label for the x axis
  percent_histogram_chart.append("text")             
      .attr("transform",
            "translate(" + ((width/2) + margin.left + (margin.right)) + " ," + 
                           (height + 35) + ")")
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
      .attr("y", function(d) { console.log(d.length); return bar_y_axis(d.length); })
      .attr("height", function(d) { return height - bar_y_axis(d.length) - margin.top; })
      .attr("width", d => Math.max(0, bar_x_axis(d.x1) - bar_x_axis(d.x0) - 1))
      .attr("fill", function(d) {return colorOfBar(d.x0);});


      //Percentage Change Line Graph
    percent_line_x.domain(d3.extent(DailyPercentageChangesWithDates, function(d, i) { return d.date; }));
    percent_line_y.domain([d3.min(DailyPercentageChangesWithDates, function(d, i) { return d.change; }), d3.max(DailyPercentageChangesWithDates, function(d, i) { return d.change; })]);

    var percent_g = percent_change_chart.append("g")
      .attr("transform", "translate(" + padding.left + "," + padding.top + ")");

        // Add the percentline path.
  percent_g.append("path")
      .data([DailyPercentageChangesWithDates])
      .attr("class", "line")
      .attr("d", percentline)
      .attr("fill", "none")
      .attr("stroke", "blue")
      .attr("stroke-width", 1.5);

    // Add the x Axis
  percent_g.append("g")
    .attr("class", "x axis")
    .attr("transform", "translate(0," + height + ")")
    .call(d3.axisBottom(percent_line_x));

    // text label for the x axis
  percent_g.append("text")             
      .attr("transform",
            "translate(" + ((width/2) + margin.left) + " ," + 
                           (height + (margin.bottom) + margin.top) + ")")
      .style("text-anchor", "middle")
      .text("Date");


    // Add the y Axis
  percent_g.append("g")
    .attr("class", "y axis")
    .attr("transform", "translate(" + margin.left + ",0)")
    .call(d3.axisLeft(percent_line_y));

  // text label for the y axis
  percent_change_chart.append("text")
      .attr("transform", "rotate(-90)")
      .attr("y", 0 + (margin.left/4))
      .attr("x",0 - (((height)/1.8) + margin.top + margin.bottom) )
      .attr("dy", "1em")
      .style("text-anchor", "middle")
      .text("Percent");


  //Shares of each stock bought graph
  // Scale the range of the data
var shares_x = d3.scaleBand()
    .range([margin.left, width]);

  // var shares_x = d3.scaleOrdinal()
  //   .range([margin.left, width]);

  var shares_x_axis = shares_x.domain(SharesOfStocks.map(function(d) { return d.stock; }));

  var shares_y = d3.scaleLinear()
    .range([height - margin.bottom, 0]);

  var max = d3.max(SharesOfStocks, function(d) { return d.amount; });

  var shares_y_axis = shares_y.domain([0, max]);

  // Add the x Axis
  shares_of_stocks_bought_chart.append("g")
      .attr("transform", "translate(" + (margin.left + (margin.right/2)) + ", " + (height - margin.top) + ")")
      .call(d3.axisBottom(shares_x));

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
   shares_of_stocks_bought_chart.selectAll("rect")
      .data(SharesOfStocks)
      .enter().append("rect")
      .attr("class", "bar")
      .attr("x", d => shares_x_axis(d.stock)  + (margin.left + (margin.right/2)))
      .attr("y", function(d) { return shares_y_axis(d.amount); })
      .attr("height", function(d) { return height - shares_y_axis(d.amount) - margin.top; })
      .attr("width", shares_x.bandwidth())
      .attr("fill", function(d) {return colorOfShares(d.amount, max);});


    text_object.append("text")
        .attr("x", ((width + margin.left)/2))             
        .attr("y", 0 + 30)
        .attr("text-anchor", "middle")  
        .style("font-size", "25px") 
        .style("text-decoration", "underline")  
        .text("Starting Money: " + startingMoney);

    text_object.append("text")
        .attr("x", ((width + margin.left)/2))             
        .attr("y", 0 + 60)
        .attr("text-anchor", "middle")  
        .style("font-size", "25px") 
        .style("text-decoration", "underline")  
        .text("Ending Money: " + (currentMoney + stockValue));

    text_object.append("text")
        .attr("x", ((width + margin.left)/2))             
        .attr("y", 0 + 90)
        .attr("text-anchor", "middle")  
        .style("font-size", "25px") 
        .style("text-decoration", "underline")  
        .text("Account Percentage Gain: " + AccountPercentageGain);

    text_object.append("text")
        .attr("x", ((width + margin.left)/2))             
        .attr("y", 0 + 120)
        .attr("text-anchor", "middle")  
        .style("font-size", "25px") 
        .style("text-decoration", "underline")  
        .text("Average Yearly Percentage Gain: " + YearlyPercentageGain);

    text_object.append("text")
        .attr("x", ((width + margin.left)/2))             
        .attr("y", 0 + 150)
        .attr("text-anchor", "middle")  
        .style("font-size", "25px") 
        .style("text-decoration", "underline")  
        .text("Standard Deviation: " + YearlyStandardDeviation);

    text_object.append("text")
        .attr("x", ((width + margin.left)/2))             
        .attr("y", 0 + 180)
        .attr("text-anchor", "middle")  
        .style("font-size", "25px") 
        .style("text-decoration", "underline")  
        .text("SPY Percentage Gain: " + SPYPercentageGain);

    text_object.append("text")
        .attr("x", ((width + margin.left)/2))             
        .attr("y", 0 + 210)
        .attr("text-anchor", "middle")  
        .style("font-size", "25px") 
        .style("text-decoration", "underline")  
        .text("Max Drawdown Percentage: " + MaxDrawdownPercentage);

    text_object.append("text")
        .attr("x", ((width + margin.left)/2))             
        .attr("y", 0 + 240)
        .attr("text-anchor", "middle")  
        .style("font-size", "25px") 
        .style("text-decoration", "underline")  
        .text("Sharpe Ratio: " + SharpeRatio);

}

// Random strategy. Probability to buy each available stock each day. Sell after 60 days of ownership.
function RandomStrategy(date, probability) {
    stocks.forEach(function(stock) {
        if(!HasPrice(date, stock))
            return;
        var value = GetPrice(date, stock);
        var roll = Math.random();
        if (roll < probability) {

            //if we got da money
            if (currentMoney > value) {
                //buy dat and log it
                purchases.push({ date: date, stock: stock, amount: 1});

                var added = false;
                //Add back to shares
                for(var i = 0; i < SharesOfStocks.length; ++i)
                {
                  if(stock == SharesOfStocks[i].stock)
                  {
                    SharesOfStocks[i].amount += 1;
                    added = true;
                  }
                }
                
                if(!added)
                {
                  SharesOfStocks.push({stock: stock, amount: 1});
                }

                currentMoney -= value;
                // console.log("Buying a share of " + stock + " on " + date + " for " + value);
            }
        }
    });

    var i = purchases.length;
    while(i--) {
        var purchase = purchases[i];
        var purchase_date = new Date(purchase.date);
        var current_date = new Date(date);

        var diff = (current_date - purchase_date) / 86400000; // convert ms (dates) to days
        if (diff > 60) {
            if(!HasPrice(date, purchase.stock))
                continue;
            var value = GetPrice(date, purchase.stock)
            currentMoney += value;
            // console.log("Selling a share of " + purchase.stock + " on " + current_date + " for " + value);
            purchases.splice(i, 1);
        }
    }
}

// Get value of all owned stocks on a certain date
function GetTotalStockValue(date){
    var result = 0;
    purchases.forEach(function(purchase) {
        result += GetPrice(date, purchase.stock)});
    // console.log(result);
    return result;
}

function CalculatePercentageGain(){
    var yesterdayMoney = DailyAccountValue[DailyAccountValue.length - 1].money;
    var result = (currentMoney + stockValue - yesterdayMoney)* 100 / yesterdayMoney;
    var firstDayMoney = DailyAccountValue[0].money;

    var change_value = (currentMoney + stockValue - firstDayMoney)* 100 / firstDayMoney;

    DailyPercentageChangesWithDates.push({ date: DailyAccountValue[DailyAccountValue.length - 1].date, percent: result, change: change_value })

    if(result > histogram_right_max)
    {
        // console.log(result);
        result = histogram_right_max;
    }
    else if(result < histogram_left_max)
    {
        // console.log(result);
        result = histogram_left_max;
    }

    DailyPercentageChanges.push(result);
    
}

function CalculateMoneyChangePerYear()
{
  var prevElement = DailyAccountValue[0];
  var moneyLost = 0;
  var moneygained = 0;
  var startingmoney2 = DailyAccountValue[0].money;

  DailyAccountValue.forEach (function(element)
  {
    var currYear = element.date.getYear() + 1900;

    if (currYear == prevElement.date.getYear() + 1900)
    {
      var money_difference = (element.money - prevElement.money);

      if (money_difference > 0)
      {
        moneygained += money_difference;
      }
      else if (money_difference < 0)
      {
        moneyLost += money_difference;
      }
    }
    else
    {
      MoneyChangePerYear.push({year: currYear, starting_money: startingmoney2, money_gained: moneygained, money_lost: moneyLost});

      startingmoney2 = element.money;
    }

    prevElement = element;
  });
}

function CalculateMoneyChangePerMonth()
{
  var prevElement = DailyAccountValue[0];
  var moneyLost = 0;
  var moneygained = 0;
  var startingmoney2 = DailyAccountValue[0].money;

  foreach (element in DailyAccountValue)
  {
    var currYear = element.date.getYear() + 1900;
    var currMonth = element.date.getMonth();

    if (currMonth == prevElement.date.getMonth())
    {
      var money_difference = (element.money - prevElement.money);

      if (money_difference > 0)
      {
        moneygained += money_difference;
      }
      else if (money_difference < 0)
      {
        moneyLost += money_difference;
      }
    }
    else
    {
      MoneyChangePerMonth.push({year: currYear, starting_money: startingmoney2, month: currMonth, money_gained: moneygained, money_lost: moneyLost});

      startingmoney2 = element.money;
    }

    prevElement = element;
  }
}

var drawdownDelta = 0;
var drawdownPeak = 0;
function CheckDrawdown(){
    var yesterdayMoney = DailyAccountValue[DailyAccountValue.length - 1].money;
    if(currentMoney + stockValue < yesterdayMoney)
    {
        drawdownDelta += currentMoney + stockValue - yesterdayMoney;
        if(drawdownDelta < MaxDrawdownPercentage)
            MaxDrawdownPercentage = drawdownDelta * 100 /drawdownPeak;
    }
    else
    {
        drawdownPeak = currentMoney + stockValue;
        drawdownDelta = 0;
    }
}

// Check if a price exists at that stock/date
function HasPrice(date, stock){
    var result = parseFloat(prices[date][stock]);
    if(Number.isNaN(result))
        return false;
    return true; 
}

// Get a price on a stock/date (returns 0 if doesn't exist)
function GetPrice(date, stock){
    var result = parseFloat(prices[date][stock]);
    if(Number.isNaN(result))
        return 0;
    return result;
}

function error(result){
    console.log("Couldn't load CSV");
    console.log("Result: " + result);
}