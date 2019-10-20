
//data load
var dates;
var stocks;
var prices;

var startingMoney = 10000;
var currentMoney = startingMoney;

//Need to calculate
var account_percentage_gain = 0.0;
var average_yearly_percentage_gain = 0.0;
var standard_deviation = 0.0;
var percentage_gain_of_spy = 0.0;
var max_drawdown_percentage = 0.0;
var sharpe_ratio = 0.0;

//Graph vars
var moneyOverTimeArray = [];
var dayPercentChange = [];

var margin = {top: 20, right: 30, bottom: 30, left: 80},
    width = 1250 - margin.left - margin.right,
    height = 550 - margin.top - margin.bottom;

// var x = d3.scaleBand()
//     .rangeRound([margin.left, width])
//     .padding(0.1);

// var xAxis = d3.axisBottom(x);

// var y = d3.scaleLinear()
//     .range([height, (margin.top + 20)]);

// var yAxis = d3.axisLeft(y);

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

var result = d3.csv("https://raw.githubusercontent.com/PvtBooth/cs399project6/master/StockDataGenerator/DATA.csv").then(function(data)
{
    // All Data Mappings
    dates = data.map(d => d["Date"]);
    stocks = Object.keys(data[0]);
    stocks.shift();
    prices = data;

    var purchases = [];

    var iterator = 0;
    var count = 0;




    dates.forEach(function(date)
    {
        stocks.forEach(function(stock)
        {
            var roll = Math.random();
            if (roll < 0.1) {

                var value = GetPrice(date, stock);
                //if we got da money
                if (value > 0 && currentMoney - value > 0.01) {
                    //buy dat and log it
                    purchases.push({ date: date, stock: stock, amount: 1});
                    currentMoney = currentMoney - value;

                    console.log("Buying a share of " + stock + " on " + date + " for " + value);
                }
            }
        });


        var i = purchases.length;
        while(i--)
        {
            var purchase = purchases[i];
            var purchase_date = new Date(purchase.date);
            var current_date = new Date(date);
            
            var diff = (current_date - purchase_date) / 86400000;
            if (diff > 60)
            {
                var value = GetPrice(date, purchase.stock);
                if(value > 0)
                {
                    currentMoney = currentMoney + value;
                    console.log("Selling a share of " + purchase.stock + " on " + current_date + " for " + value);
                    purchases.splice(i, 1);
                }
            }
        }

        // if(iterator == 0 || (iterator % 100) == 0)
        {
            moneyOverTimeArray.push({ date: new Date(date), money: currentMoney});

            //console.log("Entry: Date: " + moneyOverTimeArray[count].date + " , money: " + moneyOverTimeArray[count].money);

            count += 1;
        }

        //Calculate change between today and yesterday
        if(iterator >= 1)
        {
            var yesterdayMoney = moneyOverTimeArray[iterator - 1].money;
            //currentMoney - yesterday currentMoney
            var difference = currentMoney - yesterdayMoney;
            var percent = difference / yesterdayMoney;
            var result = percent * 100.0;

            if(result > histogram_right_max)
            {
                result = histogram_right_max;
            }
            else if(result < histogram_left_max)
            {
                result = histogram_left_max;
            }

            dayPercentChange.push(result);


            //console.log("Value: " + result);
        }
       
        iterator += 1;
    });

    //Remove loading text
    jQuery("#loading").remove();

    // CALCULATE FINAL VAL
    console.log("Finished");
    var stockValue = 0;
    purchases.forEach(function(purchase)
    {
        stockValue += GetPrice(dates[dates.length-1], purchase.stock)
    });
    console.log("Stocks Value: " + stockValue);
    console.log("Cash Value: " + currentMoney);

    //Money Graph
    // Scale the range of the data
    line_x.domain(d3.extent(moneyOverTimeArray, function(d, i) { return d.date; }));
    line_y.domain([0, d3.max(moneyOverTimeArray, function(d, i) { return d.money; })]);

    // Add the valueline path.
  money_chart.append("path")
      .data([moneyOverTimeArray])
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

var bins = histogram(dayPercentChange);

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
        .text("Ending Money: " + currentMoney);

    text_object.append("text")
        .attr("x", ((width + margin.left)/2))             
        .attr("y", 0 + 90)
        .attr("text-anchor", "middle")  
        .style("font-size", "25px") 
        .style("text-decoration", "underline")  
        .text("Account Percentage Gain: " + account_percentage_gain);

    text_object.append("text")
        .attr("x", ((width + margin.left)/2))             
        .attr("y", 0 + 120)
        .attr("text-anchor", "middle")  
        .style("font-size", "25px") 
        .style("text-decoration", "underline")  
        .text("Average Yearly Percentage Gain: " + average_yearly_percentage_gain);

    text_object.append("text")
        .attr("x", ((width + margin.left)/2))             
        .attr("y", 0 + 150)
        .attr("text-anchor", "middle")  
        .style("font-size", "25px") 
        .style("text-decoration", "underline")  
        .text("Standard Deciation: " + standard_deviation);

    text_object.append("text")
        .attr("x", ((width + margin.left)/2))             
        .attr("y", 0 + 180)
        .attr("text-anchor", "middle")  
        .style("font-size", "25px") 
        .style("text-decoration", "underline")  
        .text("SPY Percentage Gain: " + percentage_gain_of_spy);

    text_object.append("text")
        .attr("x", ((width + margin.left)/2))             
        .attr("y", 0 + 210)
        .attr("text-anchor", "middle")  
        .style("font-size", "25px") 
        .style("text-decoration", "underline")  
        .text("Max Drawdown Percentage: " + max_drawdown_percentage);

    text_object.append("text")
        .attr("x", ((width + margin.left)/2))             
        .attr("y", 0 + 240)
        .attr("text-anchor", "middle")  
        .style("font-size", "25px") 
        .style("text-decoration", "underline")  
        .text("Sharpe Ratio: " + sharpe_ratio);

});


function GetPrice(date, stock)
{
    var stockPrices = prices.map(d => d[stock]);
    var dateIndex = dates.indexOf(date);
    return parseFloat(stockPrices[dateIndex]);
}