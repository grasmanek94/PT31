#include <algorithm>
#include <string>
#include <map>
#include <iostream>

#include "Server.hxx"

namespace CommandHandler
{
	Command::Command(fn function) : function(function)
	{ }

	void Command::do_command(Server* context, const std::string& params) 
	{ 
		((*context).*function)(params, parser); 
	};

	namespace Internal
	{
		std::map<std::string, Command*> * command_map;
	};

	void Register(Command::fn handler, const std::string& command_name, const std::string& format)
	{
		if (Internal::command_map == NULL)
		{
			Internal::command_map = new std::map < std::string, Command* >;
		}
		std::string data(command_name);
		//transform command to lower case so even command names are case insensetive
		std::transform(data.begin(), data.end(), data.begin(), ::tolower);

		if (Internal::command_map->find(command_name) != Internal::command_map->end())
		{
			std::cout << "CommandHandler WARNING: Command already exists: " << data << std::endl;
		}
		Command* cmd = new Command(handler);
		cmd->parser.SetFormat(format);
		Internal::command_map->insert(std::pair<std::string, Command*>(data, cmd));
	}

	bool Execute(Server* context, const std::string& cmdtext)
	{
		//declare strings we need
		std::string command;
		std::string parameters;

		//find the space in the command
		size_t space = cmdtext.find(' ');

		if (space == std::string::npos)
		{
			//if there is no space treat the whole string as the command
			command.assign(cmdtext);
		}
		else
		{
			//else split the input into a command and parameters, with the first space being the place to split, don't include first space into the parameters string
			command.assign(cmdtext.begin(), cmdtext.begin() + space);
			parameters.assign(cmdtext.begin() + (space + 1), cmdtext.end());
		}

		//make command call lowercase characters so the command call is case-insensetive
		std::transform(command.begin(), command.end(), command.begin(), ::tolower);

		//try to find the command / check if the command exists
		auto cmd = CommandHandler::Internal::command_map->find(command);
		if (cmd != CommandHandler::Internal::command_map->end())
		{
			cmd->second->parser.ExecuteManualParse(parameters);
			cmd->second->do_command(context, parameters);
			return true;
		}
		return false;
	}
};