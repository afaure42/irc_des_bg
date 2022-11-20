#include "functionMap.hpp"

// INITIALIZATION OF MAPS OF FUNCTION POINTERS
// LOGIN MAP
// To add more functions to he map,	simply add to the map
// the pair <string, function>
// Don't forget to declare it's prototype in functionMap.hpp or it wont work
void	setFunctionMap(exec_fn_map &function_map) {
	exec_fn_map::iterator	it = function_map.begin();

	function_map.insert(
			it,	fn_map_pair("PASS", pass));
	function_map.insert(
			it,	fn_map_pair("NICK", nick));
	function_map.insert(
			it,	fn_map_pair("USER", user));
	function_map.insert(
			it, fn_map_pair("PRIVMSG", privmsg));
	function_map.insert(
			it, fn_map_pair("NOTICE", notice));
	function_map.insert(
			it, fn_map_pair("JOIN", join));
	function_map.insert(
			it, fn_map_pair("PING", ping));
	function_map.insert(
			it, fn_map_pair("PART", part));
	function_map.insert(
			it, fn_map_pair("QUIT", quit));
	function_map.insert(
			it, fn_map_pair("MODE", mode));
	function_map.insert(
			it, fn_map_pair("AWAY", away));
	function_map.insert(
			it, fn_map_pair("NAMES", names));
	function_map.insert(
			it, fn_map_pair("INVITE", invite));
	function_map.insert(
			it, fn_map_pair("LIST", list));
	function_map.insert(
			it, fn_map_pair("TOPIC", topic));
	function_map.insert(
			it, fn_map_pair("OPER", oper));
	function_map.insert(
			it, fn_map_pair("KICK", kick));
	function_map.insert(
			it, fn_map_pair("WHO", who));
	function_map.insert(
			it, fn_map_pair("die", die)); //yes wtf idk why irssi doesnt send DIE
}
