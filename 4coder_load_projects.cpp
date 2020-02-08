/*
4coder_load_projects.cpp - Command to choose between which project you want to load.
	In order to make it work, your own paths to your projects have to be hardcoded
	and each path should contain an project.4coder file.
*/

// TOP

// This command is here to show that you could use project_listers instead of load_project at startup
// Use this in your binding map function BindCore(viccarau_startup, CoreCode_Startup);

CUSTOM_COMMAND_SIG(viccarau_startup)
CUSTOM_DOC("Viccarau's command for responding to a startup event")
{
    ProfileScope(app, "viccaru startup");
    User_Input input = get_current_input(app);
    if (match_core_code(&input, CoreCode_Startup)){
        String_Const_u8_Array file_names = input.event.core.file_names;
        load_themes_default_folder(app);
        default_4coder_initialize(app, file_names);
        default_4coder_side_by_side_panels(app, file_names);
        if (global_config.automatically_load_project){
			project_listers(app);
        }
    }
}

CUSTOM_COMMAND_SIG(close_all_files)
CUSTOM_DOC("close all files that are opened no matter the extension")
{
	Buffer_ID buffer = get_buffer_next(app, 0, Access_Always);
	while (buffer != 0)
	{
		buffer_kill(app, buffer, BufferKill_AlwaysKill);
		buffer = get_buffer_next(app, buffer, Access_Always);
	}
}

CUSTOM_UI_COMMAND_SIG(projects_lister)
CUSTOM_DOC("List of all your projects")
{
	Scratch_Block scratch(app);
    char *query = "Load your project:";

    Lister *lister = begin_lister(app, scratch);
    lister_set_query(lister, query);
    lister->handlers = lister_get_default_handlers();

	// Here are the hardcoded paths that you should change depending on your projects
	String_Const_u8 project1 = string_u8_litexpr("d:/Work/bisqwit/");
	String_Const_u8 project2 = string_u8_litexpr("d:/Work/4/");
	String_Const_u8 project3 = string_u8_litexpr("d:/Work/doom-3d/");
	String_Const_u8 project4 = string_u8_litexpr("d:/Work/doom_nukem/");

	code_index_lock();

	// Here are the names you want to give to each of your projects (optional)

	lister_add_item(lister, project1, string_u8_litexpr("bisqwit"), &project1, 0);
	lister_add_item(lister, project2, string_u8_litexpr("4"), &project2, 0);
	lister_add_item(lister, project3, string_u8_litexpr("Doom 3d"), &project3, 0);
	lister_add_item(lister, project4, string_u8_litexpr("Doom Nukem"), &project4, 0);
	code_index_unlock();

	Lister_Result l_result = run_lister(app, lister);
	String_Const_u8 *result = 0;
    if (!l_result.canceled && l_result.user_data != 0){
		result = (String_Const_u8 *)l_result.user_data;
		if (result->str != 0){
			set_hot_directory(app, *result);
			close_all_files(app);
			load_project(app);
		}
	}
}

// BOTTOM