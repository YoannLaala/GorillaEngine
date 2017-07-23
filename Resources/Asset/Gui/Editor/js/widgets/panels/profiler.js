class ProfilerPanel extends Panel
{
    constructor(layoutManager)
    {
        super(layoutManager, "Profiler",
        [
            {
                name: 'Activate',
                onClick: function () 
                { 
                    Gorilla.Profiler.activate();
                }
            },
            {
                name: 'Deactivate',
                onClick: function () 
                { 
                    Gorilla.Profiler.deactivate();
                }
            },
            {
                name: 'Clear',
                onClick: function () 
                { 
                    Editor.panels.profiler.dom.html(""); 
                    Editor.panels.profiler.logs = {};
                }
            },
        ]);

        var self = this;
        this.frame = 0;

        function addChartValue(chart, value, label, color)
        {
            if(color == null) color = 'rgba(255, 255, 255, 1.0f)';

            // Limit
            var dataset = chart.data.datasets[0];
            if(dataset.data.length > chart.options.scales.xAxes[0].ticks.maxTicksLimit)
            {
                chart.data.labels.shift();
                dataset.data.shift();
                dataset.backgroundColor.shift();
            }

            // Add new value
            chart.data.labels.push(label);
            dataset.data.push(value);
            dataset.backgroundColor.push(color);
   
            chart.update();
        }

        this.onLoaded = function()
        {
            Gorilla.Template.show(self.dom, "menu_vitem", 
            {
                items:
                [
                    { name: "Frame Rate", active: "active" },
                    { name: "Frame Time" },
                    { name: "Scoped Time" },
                    { name: "Memory" },
                ]
            });

		    var canvas = document.createElement('canvas');
		    var ctx = canvas.getContext('2d');
            self.dom.append(canvas);

            self.memory = new Chart(ctx, 
            {
			    type: 'doughnut',
			    data: 
                {
				    datasets: 
                    [
                        {
                            data : [],
                            backgroundColor: [],
                            fill: false,
				        }
                    ],
                    labels: [],
                       
			    },
			    options: 
                {
				    responsive: true,
                    maintainAspectRatio: false,
                    legend: 
                    {
    	                display: true
                    },
                    scales: 
                    {
                        xAxes: 
                        [
                            {
                                display : false,
                            }
                        ],
                        yAxes: 
                        [
                            {
                                display : false,
                            }
                        ]
                    },
                    tooltips :
                    {
                        enabled : true   
                    }
			    }
		    })
            var domains = 
            [
                {label: "Animation", value: 30, color: "rgba(255,0,0,1.0)"},
                {label: "Physics", value: 10},
                {label: "User Interface", value: 10},
                {label: "Textures", value: 50},
            ];
            for(var index in domains)
            {
                var domain = domains[index];
                addChartValue(self.memory, domain.value, domain.label, domain.color);
            }

		    /*self.charts = 
            {
                framerate : new Chart(ctx, 
                {
			        type: 'bar',
			        data: 
                    {
				        datasets: 
                        [
                            {
                                data : [],
                                backgroundColor: [],
                                fill: false,
				            }
                        ],
                        labels: [],
                       
			        },
			        options: 
                    {
				        responsive: true,
                        maintainAspectRatio: false,
                        legend: 
                        {
    	                    display: false
                        },
                        scales: 
                        {
                            xAxes: 
                            [
                                {
                                    barPercentage: 1.0,
                                    categoryPercentage: 1.0,
                                    ticks: 
                                    {
                                        maxTicksLimit: 59,
                                        autoSkip: false,
					                    beginAtZero: false,
                                        callback: function(value, index, values) 
                                        {     
                                            var date = new Date((self.frame + index) * 1000);
                                            var minute = date.getMinutes().toString();
                                            if(minute.length == 1) minute = "0" + minute;
                                            var second = date.getSeconds().toString();
                                            if(second.length == 1) second = "0" + second;

                                            return minute + ":" + second;
                                        }
                                    } 
                                }
                            ],
                            yAxes: 
                            [
                                {
                                    ticks: 
                                    {
                                        stepSize : 5,
                                        min : 0,
                                        max : 80,
					                    beginAtZero: true
                                    }
                                }
                            ]
                        },
                        tooltips :
                        {
                            enabled : false   
                        }
			        }
		        })
            }*/
        }

        var colors = ["rgba(75, 192, 192, 0.2)", "rgba(255, 159, 64, 0.2)", "rgba(255, 99, 132, 0.2)"];
        this.onChanged = function(string)
        {
            //var chart = self.charts.framerate;
            //var profile = JSON.parse(string); 
            //var fps = profile.fps;

            //var color = null;
            //if(fps>28) color = colors[0];
            //else if(fps>20) color = colors[1];
            //else color = colors[2];
                
            //// Limit charts
            //var values =  chart.data.datasets[0];
            //if(values.data.length > chart.options.scales.xAxes[0].ticks.maxTicksLimit)
            //{
            //    chart.data.labels.shift();
            //    values.data.shift();
            //    values.backgroundColor.shift();
            //    ++self.frame;
            //}

            //// Add new value
            //chart.data.labels.push(0);
            //values.data.push(fps);
            //values.backgroundColor.push(color);
   
            //chart.update();
        }
    }
}