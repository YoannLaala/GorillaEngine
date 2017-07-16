{ 
	"Topology": "TriangleList",
	"RenderState":
	{
		"WireFrame" : false,
		"Cull" : false,
		"Clockwise" : true,
		"Scissor" : false,
		"MultiSample" : false,	
		"Depth":
		{
			"Enabled" : false,
			"Write" : false,
			"Comparaison" : "Never"
		},
		"Stencil" : false,	
		"Blend" : 
		{
			"Enabled" : true,
			"Color" :
			{
				"Source" : "One",
				"Operation" : "Add",
				"Destination" : "Zero"
			},
			"Alpha" :
			{
				"Source" : "One",
				"Operation" : "Add",
				"Destination" : "Zero"
			}
		}
	}
}