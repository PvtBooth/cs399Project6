
//data load
var dates;
var stocks;
var prices;

var startingMoney = 10000;
var currentMoney = startingMoney;

var result = d3.csv("DATA.csv").then(function(data)
{
    // All Data Mappings
    dates = data.map(d => d["Date"]);
    stocks = Object.keys(data[0]);
    stocks.shift();
    prices = data;

    var purchases = [];
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



    });

    // CALCULATE FINAL VAL
    console.log("Finished");
    var stockValue = 0;
    purchases.forEach(function(purchase)
    {
        stockValue += GetPrice(dates[dates.length-1], purchase.stock)
    });
    console.log("Stocks Value: " + stockValue);
    console.log("Cash Value: " + currentMoney);
});


function GetPrice(date, stock)
{
    var stockPrices = prices.map(d => d[stock]);
    var dateIndex = dates.indexOf(date);
    return parseFloat(stockPrices[dateIndex]);
}