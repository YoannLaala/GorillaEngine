function GorillaMaterialEditor(workspaceDom, listDom, events)
{
	ENODE = 
	{
		INPUT : 0,
		FUNCTION : 1,
		OUTPUT : 2,
	};
	
	EBUFFER = 
	{
		NUMBER : 0,
	};
	
	EFLAG = 
	{
		SHADER_CHANGED : 1 << 0,
	};
	_flag = 0;
	
	_outputCell = undefined;
	_graph = undefined;
	_paper = undefined;
	_shaderGenerator = 
	{	
		template: '#include "Geometry.vs"\n$[BUFFER_CODE] PS_OUTPUT PixelDefault(VS_OUTPUT _input) { PS_OUTPUT output = (PS_OUTPUT)0; $[FUNCTION_CODE] return output; }',
		buffers:
		[
			{
				struct: "cbuffer ConstantBufferNumber",
				type: "float4",
				semantic: "Number",
				size: 4,
				values:[],
				addValue: function(value)
				{
					var offset = this.values.length;
					var bufferIndex = parseInt(offset / this.size);
					this.values.push(value);
					
					return this.semantic + bufferIndex.toString() + "." + ("xyzw"[offset % this.size]);
				}
			}
		],
		execute: function()
		{
			//!	@brief		Generate a shader code from a specfified cell
			//!	@date		2015-04-04
			function generateCodeFromCell(cell)
			{
				var shader = "";
				var cells = _graph.getPredecessors(cell);
				for (var index = 0; index < cells.length; ++index) 
				{
					shader += generateCodeFromCell(cells[index]);
				}
				
				// generate on node non exposed
				var node = cell.attributes.prop;
				if(!node.exposed)
				{
					shader += node.descriptor.shader.generate(node);
				}
				
				return shader;
			}
			
			// Clear buffers
			for(var index = 0; index < this.buffers.length; ++index)
			{
				this.buffers[index].values = [];
			}
			
			// generate function code
			var functionCode = generateCodeFromCell(_outputCell);
			
			// generate buffer code
			var bufferCode = "";
			for(var bufferIndex = 0; bufferIndex < this.buffers.length; ++bufferIndex)
			{
				var buffer = this.buffers[bufferIndex];
				bufferCode += buffer.struct;
				bufferCode += "{"	
					// foreach block of value
					var variableCount = parseInt(buffer.values.length / buffer.size)+1;
					for(var index = 0; index < variableCount; ++index)
					{
						bufferCode += buffer.type + " " + buffer.semantic + index.toString() + ";";
					}
				bufferCode += "};"
			}
			
			// generate complete shader from template
			var shader = this.template;
			shader = shader.replace("$[BUFFER_CODE]", bufferCode);
			shader = shader.replace("$[FUNCTION_CODE]", functionCode);
						
			return shader;
		},
		getValue: function(instance, index)
		{
			var subInstance = instance.value[index];
			if(subInstance != undefined)
			{
				// handle exposed value
				if(subInstance.exposed)
				{
					var bufferType = subInstance.descriptor.shader.buffer;
					var buffer = this.buffers[bufferType];
					
					return buffer.addValue(subInstance.value);
				}
				return subInstance.id;
			}
			
			return instance.descriptor.shader.default[index];
		},
		
		getFirstType: function(instance)
		{
			var array = instance.value;
			for (var index = 0; index < array.length; ++index) 
			{
				var subInstance = array[index];
				if(subInstance != undefined)
				{
					return subInstance.descriptor.shader.type;
				}
			}
			
			return "float";
		}
	}
	
	//!	@brief		Describe all nodes
	//!	@date		2015-04-04
	_nodes = [
			
		{
			"name" : "Input",
			"descriptors" :
			[
				{
					"name" : "Number",
					"outputs" : [ "" ],
					"shader":
					{
						"buffer" : EBUFFER.NUMBER,
						"default" : 0.0,
						"generate" : function(instance) { return "float " + instance.id + " = " + instance.value + ";"; }
					}
				},
				{
					"name" : "Texture",
					"inputs" : [ "UV" ],
					"outputs" : [ "RGBA", "R", "G", "B", "A" ],
					"shader":
					{
						"buffer" : EBUFFER.NUMBER,
						"default" : 0.0,
						"generate" : function(instance) { return "float4 " + instance.id + " = " + instance.value + ";"; }
					}
				}
			]
		},
		{
			"name" : "Function",
			"descriptors" :
			[
				{
					"name" : "Add",
					"inputs" : [ "A", "B" ],
					"outputs" : [ "" ],
					"shader":
					{
						"default" : [ 0, 0],
						"generate" : function(instance) 
						{ 
							var type = _shaderGenerator.getFirstType(instance);
							return type + " " + instance.id + " = " + _shaderGenerator.getValue(instance, 0) + "+" + _shaderGenerator.getValue(instance, 1) + ";";
						}
					}
				}
			]
		},	
		{
			"name" : "Output",
			"descriptors" :
			[
				{
					"name" : "Output",
					"inputs" : [ "Color", "Normal" ],
					"outputs" : [ ],
					"shader":
					{
						"default" : [ 0, "_input.Normal"],
						"generate" : function(instance) 
						{ 		
							var outputName = "output.";
						
							var code = outputName + "Color = (float4)" + _shaderGenerator.getValue(instance, 0) + ";";	
							code += outputName + "Normal = (float4)" + _shaderGenerator.getValue(instance, 1) + ";";

							return code; 
						}
					}
				}
			]
		}
	];
	
	//!	@brief		Flag helper
	//!	@date		2015-04-04
	function hasFlag(flag) { return (_flag & flag) != 0; }
	function setFlag(flag) { _flag |= flag; }
	function removeFlag(flag) { _flag &= ~flag; }
	
	//!	@brief		Enum NODE Type
	//!	@date		2015-04-04
	function createNodeInstance(id, descriptor, value, exposable)
	{
		var instance =
		{ 
			id: id, 
			name: descriptor.name,
			descriptor: descriptor, 
			value: value, 
			exposed: exposable 
		};
		
		return instance;
	}
	
	//!	@brief		Add a cell in the graph
	//!	@date		2015-04-04
	function addCell(type, index)
	{			
		// get proper list
		var descriptor = _nodes[type].descriptors[index];
		
		// create a new cell
		var cell = new joint.shapes.devs.Atomic(
		{
			position: { x: 0, y: 0 },
			inPorts: descriptor.inputs,
			outPorts: descriptor.outputs,
			attrs: 
			{
				'.label' : { text : descriptor.name },
				'.inPorts circle': { magnet: 'passive', type: 'input' },
				'.outPorts circle': { type: 'output' }
			}
		});
		
		// create default values if needed
		var value = descriptor.shader.default;
		var exposable = (type == ENODE.INPUT);
		if(type != ENODE.INPUT)
		{
			value = descriptor.shader.default.slice();
			for (var index = 0; index < value.length; ++index) 
			{
				value[index] = undefined;
			}
		}	
		
		// set the prop of the shape and add it to the graph
		cell.attributes.prop = createNodeInstance(cell.cid, descriptor, value, exposable);
		_graph.addCell(cell);
		
		return cell;
	}
	
	//!	@brief		Set a cell value
	//!	@date		2015-04-04
	function setCellValue(id, port, value)
	{		
		// retrieve target
		var cellTarget = _graph.getCell(id);
		var port_index = cellTarget.attributes.inPorts.indexOf(port);
		
		// retrieve the value associed to the id
		if(value != undefined)
		{
			var cellSource = _graph.getCell(value);
			value = cellSource.attributes.prop;			
		}
		
		// affect the value
		cellTarget.attributes.prop.value[port_index] = value;
	}
	
	//!	@brief		handle generic event for the workspace
	//!	@date		2015-04-04
	_graph = new joint.dia.Graph();
	_graph.on(
	{
		"add" : function(cell)
		{
			if(!cell.isLink())
			{
				events.onSelectionChanged(cell.attributes.prop);
				return;
			}
			
			// prepare event on link edition
			cell.on(
			{
				"change:target": function()
				{						
					// the link has a proper target
					var target = this.attributes.target;
					if(target.id != undefined)
					{							
						// no changes detected
						if(this.lastTarget != undefined)
						{
							if(target.id == this.lastTarget.id && target.port == this.lastTarget.port)
							{
								return;
							}
							
							// clean old node value
							setCellValue(this.lastTarget.id, this.lastTarget.port, undefined);
						}
						
						// update new value
						setCellValue(target.id, target.port, this.attributes.source.id);
						this.lastTarget = { id: target.id, port: target.port};
						
						// Notify the shader changed
						setFlag(EFLAG.SHADER_CHANGED);
					}
				}
			});
		},
		"remove" : function(cell)
		{
			if(cell.isLink())
			{
				if(cell.lastTarget != undefined)
				{
					setCellValue(cell.lastTarget.id, cell.lastTarget.port, undefined);
					
					// Notify the shader changed
					setFlag(EFLAG.SHADER_CHANGED);
				}
			}
		}
	});
	
	// Create workspace in the specified dom
	var workspaceElement = $(workspaceDom);
	_paper = new joint.dia.Paper(
	{
		el: workspaceElement,
		width: workspaceElement.width(),
		height: workspaceElement.height(),
		gridSize: 1,
		model: _graph,
		snapLinks: true,
		linkPinning: false,
		embeddingMode: true,
		clickThreshold: 1,
		snapLinks: { radius: 75 },
		validateEmbedding: function(childCell, parentCell) 
		{
			window.console.log("[validateEmbedding]");
			return true;//parentView.model instanceof joint.shapes.devs.Coupled;
		},
		validateConnection: function(cellSource, magnetSource, cellTarget, magnetTarget, end, cellLink) 
		{
			// prevent linking from input ports & linking element on itself
			return magnetSource && magnetSource.getAttribute('type') !== 'input' && cellSource != cellTarget;
		}
	});
	
	_paper.on(
	{
		"cell:pointerup" : function(cell, x, y)
		{ 
			// generate new shader if needed
			if(hasFlag(EFLAG.SHADER_CHANGED))
			{
				var shader = _shaderGenerator.execute();
				events.onShaderChanged(shader, _shaderGenerator.buffers[0].values);
				removeFlag(EFLAG.SHADER_CHANGED);
			}
		},
		"cell:pointerclick" : function(cell, x, y)
		{ 
			events.onSelectionChanged(cell.model.attributes.prop);
		},
		"blank:pointerclick" : function(cell, x, y) 
		{ 
			events.onSelectionChanged(undefined);
		}	
	});
	
	// Create all nodes available in the specified dom		
	var listElement = $(listDom);
	for (var nodeTypeIndex = 0; nodeTypeIndex < _nodes.length; ++nodeTypeIndex) 
	{
		var node_type = _nodes[nodeTypeIndex];
		listElement.append(node_type.name);	
		listElement.append("<hr/>");	
		
		var descriptors = node_type.descriptors;
		for (var descriptorIndex = 0; descriptorIndex < descriptors.length; ++descriptorIndex) 
		{
			var descriptor = descriptors[descriptorIndex];
			var nodeElement = $('<button type="button" nodeType="' + nodeTypeIndex  + '" nodeIndex="' + descriptorIndex  + '">' + descriptor.name + '</button>');
			nodeElement.click(function()
			{
				addCell($(this).attr("nodeType"), $(this).attr("nodeIndex"));
			});
			listElement.append(nodeElement);
		}
		listElement.append("<br/>");
	}
	
	// Create output default node
	_outputCell = addCell(ENODE.OUTPUT,0);
}