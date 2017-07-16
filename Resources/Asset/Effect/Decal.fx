{ 
	"Topology": "TriangleList",
	"RenderState":
	{
		"WireFrame" : false,
		"Cull" : false,
		"Clockwise" : false,
		"Scissor" : false,
		"MultiSample" : false,
		"Depth":
		{
			"Enabled" : true,
			"Write" : false,
			"Comparaison" : "Less"
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