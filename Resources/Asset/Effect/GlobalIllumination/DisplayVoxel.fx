{ 
	"Topology": "TriangleList",
	"RenderState":
	{
		"WireFrame" : false,
		"Cull" : true,
		"Clockwise" : false,
		"Scissor" : false,
		"MultiSample" : false,
		"Depth":
		{
			"Enabled" : true,
			"Write" : true,
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