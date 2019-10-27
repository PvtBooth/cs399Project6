
d3.csv("https://raw.githubusercontent.com/PvtBooth/cs399project6/master/StockDataGenerator/DATA.csv").then(main, error);

// Main Data
var dates;
var stocks;
var prices;

// Simulation Data
var startDate = "2009-01-02";
var endDate = "2019-10-11";

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

var margin = {top: 20, right: 30, bottom: 30, left: 80},
    width = 1250 - margin.left - margin.right,
    height = 550 - margin.top - margin.bottom;

// set the ranges
var line_x = d3.scaleTime().range([margin.left, width]);
var line_y = d3.scaleLinear().range([height - 35, (margin.top + 20)]);

// define the line
var valueline = d3.line()
    .x(function(d) { return line_x(d.date); })
    .y(function(d) { return line_y(d.money); });

var money_chart = d3.select("body").append("svg")
    .attr("width", width + margin.left + margin.right)
    .attr("height", height + margin.top + margin.bottom)
    .append("g")
    // .attr("transform",
    //       "translate(" + margin.left + "," + margin.top + ")");

var percent_histogram_chart = d3.select("body").append("svg")
    .attr("width", width + margin.left + margin.right)
    .attr("height", height + margin.top + margin.bottom)
    .append("g")
    // .attr("transform",
    //       "translate(" + margin.left + "," + margin.top + ")");

var text_object = d3.select("body").append("svg")
    .attr("width", width + margin.left + margin.right)
    .attr("height", height + margin.top + margin.bottom)
    .append("g")

var histogram_left_max = -8.0;
var histogram_right_max = 8.0;

function main(data){
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
        console.log(date);
        //RandomStrategy(date, 0.5); // almost 5 stocks per day
        
        LinearlyWeightedMovingAverage(date, 5, 0.01);
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
    jQuery("#loading").remove();

    //Money Graph
    // Scale the range of the data
    line_x.domain(d3.extent(DailyAccountValue, function(d, i) { return d.date; }));
    line_y.domain([0, d3.max(DailyAccountValue, function(d, i) { return d.money; })]);

    // Add the valueline path.
  money_chart.append("path")
      .data([DailyAccountValue])
      .attr("class", "line")
      .attr("d", valueline)
      .attr("fill", "none")
      .attr("stroke", "green")
      .attr("stroke-width", 1.5);

    // Add the x Axis
  money_chart.append("g")
      .attr("transform", "translate(0," + (height - 35) + ")")
      .call(d3.axisBottom(line_x));

    // text label for the x axis
  money_chart.append("text")             
      .attr("transform",
            "translate(" + ((width + margin.left)/2) + " ," + 
                           (height) + ")")
      .style("text-anchor", "middle")
      .text("Date");

    // Add the y Axis
  money_chart.append("g")
      .attr("transform", "translate(" + (margin.left) + ",  0)")
      .call(d3.axisLeft(line_y));

  // text label for the y axis
  money_chart.append("text")
      .attr("transform", "rotate(-90)")
      .attr("y", 0 + (margin.left/4))
      .attr("x",0 - (height/2))
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
    .range([height - margin.bottom, margin.top]);

var bar_y_axis = bar_y.domain([0, d3.max(bins, function(d) { return d.length; })]);

 // Add the x Axis
  percent_histogram_chart.append("g")
      .attr("transform", "translate(0," + (height) + ")")
      .call(d3.axisBottom(bar_x).ticks(histogram_right_max * 4));

    // text label for the x axis
  percent_histogram_chart.append("text")             
      .attr("transform",
            "translate(" + ((width + margin.left)/2) + " ," + 
                           (height + 35) + ")")
      .style("text-anchor", "middle")
      .text("Bins");

    // Add the y Axis
  percent_histogram_chart.append("g")
      .attr("transform", "translate(" + (margin.left) + ", " + margin.top + ")")
      .call(d3.axisLeft(bar_y));

  // text label for the y axis
  percent_histogram_chart.append("text")
      .attr("transform", "rotate(-90)")
      .attr("y", 0 + (margin.left/4))
      .attr("x",0 - (height/2))
      .attr("dy", "1em")
      .style("text-anchor", "middle")
      .text("Days");


   // Add the valueBar bar
   percent_histogram_chart.selectAll(".bar")
      .data(bins)
      .enter().append("rect")
      .attr("class", "bar")
      .attr("x", d => bar_x_axis(d.x0) + 1)
      .attr("y", function(d) { return bar_y_axis(d.length); })
      .attr("height", function(d) { return height - bar_y_axis(d.length); })
      .attr("width", d => Math.max(0, bar_x_axis(d.x1) - bar_x_axis(d.x0) - 1));


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

var DayCounter = 0;
// Random strategy. Probability to buy each available stock each day. Sell after 60 days of ownership.
function LinearlyWeightedMovingAverage(date, trendRange, threshold) {
    stocks.forEach(function(stock) {
        var val = GetPrice(date, stock);
        var dateIndex = dates.indexOf(date);
        var denominator = (trendRange*(trendRange+1)/2);
        var currentP = 0;  var prevP = 0;
        // Get today's average
        for(var i = 0; i < trendRange; i++)
        {
            var weight = trendRange - i;
            if(dateIndex - i < 0)
                continue;
            currentP += GetPrice(dates[dateIndex-i], stock)*weight;
            if(dateIndex - i - 1 < 0)
                continue;
            prevP += GetPrice(dates[dateIndex-i - 1], stock)*weight;
        }
        var currentP = currentP/denominator;
        var prevP = prevP/denominator;
        // Get yesterday's average
        // console.log("Prev: " + prevP + " Current: " + currentP);
        var delta = (currentP - prevP)/currentP;
        // ]console.log("delta: " + delta);
        if(delta > 0 &&  delta > threshold && currentMoney > val && val != 0)
        {
            purchases.push({ date: date, stock: stock, amount: 1});
            currentMoney -= val;
            // console.log("Buying a share of " + stock + " on " + date + " for " + val);
        }
        if(delta < 0 )  {
            var i = purchases.length;
            while(i--) {
                var purchase = purchases[i];
                if(purchase.stock != stock)
                    continue;
                var value = GetPrice(date, purchase.stock);
                currentMoney += value;
                // console.log("Selling a share of " + purchase.stock + " on " + date + " for " + value);
                purchases.splice(i, 1);
            }
        }
    }
)}



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