// Set the dimensions of the canvas / graph
var margin = {top: 30, right: 20, bottom: 30, left: 50},
    width = 600 - margin.left - margin.right,
    height = 270 - margin.top - margin.bottom;

// Parse the date / time
var parseDate = d3.time.format("%Y").parse;

// Set the ranges
var x = d3.time.scale().range([0, width]);
var y = d3.scale.linear().range([height, 0]);


   var cValue = function(d) { return d.x;},
        colors = d3.scale.category10();

// Define the axes
var xAxis = d3.svg.axis().scale(x)
    .orient("bottom").ticks(5);

var yAxis = d3.svg.axis().scale(y)
    .orient("left").ticks(5);

// Define the line
var valueline = d3.svg.line()
    .x(function(d) { return x(d.date); })
    .y(function(d) { return y(d.share); });
    
// Adds the svg canvas
var svg = d3.select("body")
    .append("svg")
        .attr("width", width + margin.left + margin.right)
        .attr("height", height + margin.top + margin.bottom)
    .append("g")
        .attr("transform", 
              "translate(" + margin.left + "," + margin.top + ")");

// Get the data
d3.csv("nasa.csv", function(error, data) {
    data.forEach(function(d) {
        d.date = parseDate(d.date);
        d.share = +d.share;
        d.party = +d.party;
    });

    // Scale the range of the data
    x.domain(d3.extent(data, function(d) { return d.date; }));
    y.domain([0, d3.max(data, function(d) { return d.share; })]);

    /*
     svg.append("linearGradient")				
        .attr("id", "line-gradient")			
        .attr("gradientUnits", "userSpaceOnUse")	
        .attr("x1", 0).attr("y1", y(0))			
        .attr("x2", 0).attr("y2", y(1000))		
      .selectAll("stop")						
        .data([								
            {offset: "0%", color: "red"},		
            {offset: "40%", color: "red"},	
            {offset: "40%", color: "black"},		
            {offset: "62%", color: "black"},		
            {offset: "62%", color: "lawngreen"},	
            {offset: "100%", color: "lawngreen"}	
        ])					
    .enter().append("stop")			
        .attr("offset", function(d) { return d.offset; })	
        .attr("stop-color", function(d) { return d.color; });
    */
    /*var lineGraph1 = svg.append("path")
        .attr("d", valueline(data.filter(function(d) {
            return d.party == 'blue';
        })))
        .attr("stroke", "blue")
        .attr("stroke-width", 2)
        .attr("fill", "none");
    
    
    var lineGraph2 = svg.append("path")
        .attr("d", valueline(data.filter(function(d) {
            return d.party == 'red';
        })))"
        .attr("stroke", "red")
        .attr("stroke-width", 2)
        .attr("fill", "none");
    
*/
    var color = d3.scale.ordinal() .domain(["1985","1986","1987","1988","1989","1990","1991","1992","1993","1994","1995","1996","1997","1998","1999","2000","2001","2002","2003","2004","2005","2006","2007","2008","2009","2010","2011","2012","2013","2014","2015"]) .range("#FF0000","#FF0000","#FF0000","#FF0000","#FF0000","#FF0000","#FF0000","#FF0000",      "#0000FF","#0000FF","#0000FF","#0000FF","#0000FF","#0000FF","#0000FF","#0000FF",      "#FF0000","#FF0000","#FF0000","#FF0000","#FF0000","#FF0000","#FF0000","#FF0000", "#0000FF","#0000FF","#0000FF","#0000FF","#0000FF","#0000FF","#0000FF","#0000FF");
    
    
    // Add the valueline path.
    svg.append("path")
        .attr("class", "line")
        //.style("stroke", function(d) { return color(d.party); })
        //.attr("fill","url(#")
       // .style("stroke", function() { // Add the colours dynamically
         //       return d.color = color(d.party); })
         //.attr('stroke-width', function(d) { return d.share; })
        //.attr('stroke', function(d) { return d.party; })
        .attr("d", valueline(data));
    
       // .attr("fill", function(d) { return (d.date > 1991 ? "orange" : "yellow"); });
        //.attr("d", ''+valueline(data)+"L0,"+y(0)+'L'+maxX+","+y(0));

    svg.selectAll("dot")
        .data(data)
      .enter().append("circle")
        .attr("r", 3.5)
        .attr("cx", function(d) { return x(d.date); })
        .attr("cy", function(d) { return y(d.share); })
        .style("fill", function (d) { return colors(cValue(d));})
        ;

        //.style("stroke", function() { // Add the colours dynamically
            // return d.color = color(d.party);});

    // Add the X Axis
    svg.append("g")
        .attr("class", "x axis")
        .attr("transform", "translate(0," + height + ")")
        .call(xAxis);

    // Add the Y Axis
    svg.append("g")
        .attr("class", "y axis")
        .call(yAxis);
    
    

});
