Gorilla.Template.files.modal_project_open = "                                                                                                                                                                           \
<div class='modal fade' id='myModal' role='dialog'>                                                                                                                                                                     \
	<div class='modal-dialog'>                                                                                                                                                                                          \
		<div class='modal-content'>                                                                                                                                                                                     \
			<div class='modal-header'>                                                                                                                                                                                  \
				{{#is_optional}}                                                                                                                                                                                        \
				<button type='button' class='close' data-dismiss='modal' aria-label='Close'><span aria-hidden='true'>&times;</span></button>                                                                            \
				{{/is_optional}}                                                                                                                                                                                        \
				<h4>Define your project</h4>                                                                                                                                                                            \
			</div>                                                                                                                                                                                                      \
			<ul class='nav nav-tabs'>                                                                                                                                                                                   \
				<li class='active'><a data-toggle='tab' href='#create_project'><span class='glyphicon glyphicon-file'></span> Create a project</a></li>                                                                 \
				<li><a data-toggle='tab' href='#open_project'><span class='glyphicon glyphicon-folder-open'></span> Open an existing project</a></li>                                                                   \
			</ul>                                                                                                                                                                                                       \
			<div class='modal-body' style='padding:40px 50px;'>                                                                                                                                                         \
				<div class='tab-content'>                                                                                                                                                                               \
					<div id='create_project' class='tab-pane fade in active'>                                                                                                                                           \
						<div class='form-group'>                                                                                                                                                                        \
							<label><span class='glyphicon glyphicon-tag'></span> Name</label>                                                                                                                           \
							<input type='text' class='form-control' placeholder='Enter project name'>                                                                                                                   \
						</div>                                                                                                                                                                                          \
						<br/>                                                                                                                                                                                           \
						<div class='form-group'>                                                                                                                                                                        \
							<label><span class='glyphicon glyphicon-hdd'></span> Output Directory</label>                                                                                                               \
							{{{input_new}}}                                                                                                                                                                             \
						</div>                                                                                                                                                                                          \
					</div>                                                                                                                                                                                              \
					<div id='open_project' class='tab-pane fade'>                                                                                                                                                       \
						<div class='form-group'>                                                                                                                                                                        \
							<label for='psw'><span class='glyphicon glyphicon-open-file'></span> Project file</label>                                                                                                   \
							{{{input_open}}}                                                                                                                                                                            \
						</div>                                                                                                                                                                                          \
					</div>                                                                                                                                                                                              \
				</div>                                                                                                                                                                                                  \
			</div>                                                                                                                                                                                                      \
			<div class='modal-footer'>                                                                                                                                                                                  \
				<button type='submit' class='btn btn-default btn-small'>Continue</button>                                                                                                                               \
			</div>                                                                                                                                                                                                      \
		</div>                                                                                                                                                                                                          \
	</div>                                                                                                                                                                                                              \
</div>"

Gorilla.Template.files.modal_project_build = "                                                                                                                  														\
<div class='modal fade' id='myModal' role='dialog'>                                                                                                                                                                     \
	<div class='modal-dialog'>                                                                                                                                                                                          \
		<div class='modal-content'>                                                                                                                                                                                     \
			<div class='modal-header'>                                                                                                                                                                                  \
				<button type='button' class='close' data-dismiss='modal' aria-label='Close'><span aria-hidden='true'>&times;</span></button>                                                                            \
				<h4>Build your project</h4>                                                                                                                                                                             \
			</div>                                                                                                                                                                                                      \
			<div class='modal-body'>                                                                                                                                                                                    \
				<div class='form-group'>                                                                                                                                                                                \
					<label><span class='glyphicon glyphicon-hdd'></span> Directory</label>                                                                                                                              \
					{{{input_new}}}                                                                                                                                                                                     \
				</div>                                                                                                                                                                                                  \
			</div>                                                                                                                                                                                                      \
			<div class='modal-footer'>                                                                                                                                                                                  \
				<button type='submit' class='btn btn-default btn-small'>Build</button>                                                                                                                                  \
			</div>                                                                                                                                                                                                      \
		</div>                                                                                                                                                                                                          \
	</div>                                                                                                                                                                                                              \
</div>"                                                                                                                                                                                                                 