
var margin = {top: 20, right: 30, bottom: 30, left: 180},
    width = 1250 - margin.left - margin.right,
    height = 500 - margin.top - margin.bottom;

var x = d3.scaleBand()
    .rangeRound([margin.left, width])
    .padding(0.1);

var y = d3.scaleLinear()
    .range([height, (margin.top + 20)]);

var xAxis = d3.axisBottom(x);

var p = Math.max(0, d3.precisionFixed(0.05) - 2),
    f = d3.format("." + p + "%");

var yAxis = d3.axisLeft(y).tickFormat(f);

var chart = d3.select(".chart")
    .attr("width", width + margin.left + margin.right)
    .attr("height", height + margin.top + margin.bottom)

//d3.csv("https://raw.githubusercontent.com/PvtBooth/cs399/master/data.csv", type,
//  function(error, data)
d3.csv("https://raw.githubusercontent.com/PvtBooth/cs399/master/data.csv").then(function(data)
{
  console.log(data);

   x.domain(data.map(function(d) { return d.age; }));
   y.domain([0, d3.max(data, function(d) { return +d["hallucinogen-use"]/100; })]);

chart.append("g")
      .attr("class", "x axis")
      .attr("transform", "translate(0," + height + ")")
      .call(xAxis);

  chart.append("g")
      .attr("class", "y axis")
      .attr("transform", "translate(" + (margin.left) + ",  0)")
      .call(yAxis);

  chart.selectAll(".bar")
      .data(data)
    .enter().append("rect")
      .attr("class", "bar")
      .attr("x", function(d) { return x(d.age); })
      .attr("y", function(d) { return y(+d["hallucinogen-use"]/100); })
      .attr("height", function(d) { return height - y(+d["hallucinogen-use"]/100); })
      .attr("width", x.bandwidth());

chart.append("text")
        .attr("x", (width / 2) + 87)             
        .attr("y", 0 + (margin.top))
        .attr("text-anchor", "middle")  
        .style("font-size", "25px") 
        .style("text-decoration", "underline")  
        .text("Hallucinogen Use by Age");

  chart.append("text")
        .attr("x",  80)             
        .attr("y", 0 + (height / 2))
        .attr("text-anchor", "middle")  
        .style("font-size", "16px") 
        .text("Percentage of Users");

  chart.append("text")
  .attr("x",  (width / 2) + 87)             
  .attr("y", 0 + height + (margin.bottom / 0.8))
  .attr("text-anchor", "middle")  
  .style("font-size", "16px") 
  .text("Age");
  
});
