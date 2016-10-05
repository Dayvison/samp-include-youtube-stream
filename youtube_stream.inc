#if defined _youtube_included
	#endinput
#endif
#define _youtube_included
/**
 * Play an youtube 'audio stream'
 * 
 * @note usually the API fails in newly videos...
 *
 * @version		1.0.0
 * @author		Dayvison
 * @date		04-10-2016
 */

#include <a_http>
#tryinclude <regex> // [Plugin] Regular expression by Fro1sha: http://forum.sa-mp.com/showthread.php?t=247893
#if defined _regex_included
	#define USE_REGEX
#endif
static
	Float:youtube_posX[MAX_PLAYERS],
	Float:youtube_posY[MAX_PLAYERS],
	Float:youtube_posZ[MAX_PLAYERS],
	Float:youtube_distance[MAX_PLAYERS],
	youtube_usepos[MAX_PLAYERS]
;
#if defined OnPlayYoutubeUrl
/**
 * Called when youtube url an played.
 *
 * @param      playerid  The playerid
 * @param      title     The title of url(if invalid \0)
 * @param      len       The length of url(if invalid 0)
 * @param      response  The response of url 
 * 							url codes:
 * 							1 Sucess
 * 							-1 Invalid url
 * 							-2 Invalid page
 */
	forward OnPlayYoutubeUrl(playerid, title[], len, response);
#endif

forward YoutubeResponse(playerid, response, data[]);
public YoutubeResponse(playerid, response, data[])
{
	if(response == 200)
	{
		if(strfind(data, "No video was found") != -1 || strfind(data, "<meta http-equiv=") != -1)
		{
			#if defined OnPlayYoutubeUrl
				OnPlayYoutubeUrl(playerid, "\0", 0, -1);
			#endif
			return;
		}
		new
			title[60],
			len[5],
			Find
		;
		Find = strfind(data, "<br/>");
		strmid(title, data, 7, Find-1);
		strdel(data, 0, Find+5);

		Find = strfind(data, "<br/>");
		strmid(len, data, 8, Find-1);
		strdel(data, 0, Find+11);
		PlayAudioStreamForPlayer(playerid, data, youtube_posX[playerid], youtube_posY[playerid], youtube_posZ[playerid], youtube_distance[playerid], youtube_usepos[playerid]);
		#if defined OnPlayYoutubeUrl
			OnPlayYoutubeUrl(playerid, title, strval(len), 1);
		#endif
	}
	else
	{
		#if defined OnPlayYoutubeUrl
			OnPlayYoutubeUrl(playerid, "\0", 0, -2);
		#endif
	}
}
#if defined USE_REGEX
/**
 * Determines if valid youtube url.
 *
 * @param      url   The url
 *
 * @author		Dayvison
 * @date		04-10-2016
 * @return		True if is a valid url, false not.
 */
stock IsValidYoutubeUrl(const url[])
{
	static RegEx:rHex;
	if(!rHex)
	{		
		rHex = regex_build(\"((http(s)?:\/\/)?)(www\.)?((youtube\.com\/)|(youtu.be\/))[\S]+");
	}
	return regex_match_exid(url, rHex);
}
#endif
/**
 * Converts an youtu.be url to youtube.com
 *
 * @param      Url   The url to convert
 * @param      size  The size
 *
 * @author		Dayvison
 * @date		04-10-2016
 * @return		True if is conver have sucess, false not.
 */
stock ConvertYoutubeUrl(Url[], size = sizeof(Url))
{
	new find;
	if((find = strfind(Url, "youtu.be/")) == -1)
		return false;
	strdel(Url, 0, find + 4);
	format(Url, size, "https://www.youtube.com/watch?v=", Url);
	return true;
}

/**
 * Play an youtube 'audio stream' for a player.
 *
 * @param      playerid  The ID of the player to play the audio for.
 * @param      url       The url to play.
 * @param      posX      The X position at which to play the audio. Default 0.0. Has no effect unless usepos is set to 1.
 * @param      posY      The Y position at which to play the audio. Default 0.0. Has no effect unless usepos is set to 1.
 * @param      posZ      The Z position at which to play the audio. Default 0.0. Has no effect unless usepos is set to 1.
 * @param      distance  The distance over which the audio will be heard. Has no effect unless usepos is set to 1.
 * @param      usepos    Use the positions and distance specified. Default disabled (0).
 *
 * @author		Dayvison
 * @date		04-10-2016
 * @return		True on sucess, or false on failure. if USE_REGEX active is more efficient
 */
stock PlayYoutubeForPlayer(playerid, url[], Float:posX = 0.0, Float:posY = 0.0, Float:posZ = 0.0, Float:distance = 50.0, usepos = 0)
{
	if(!IsPlayerConnected(playerid))
		return false;
	#if defined USE_REGEX
	if(!IsValidYoutubeUrl(url))
	{
		if(!ConvertYoutubeUrl(url, 256))
			return false;
	}
	#else
	if(strfind(url, "youtu.be/") != -1)
		if(!ConvertYoutubeUrl(url, 256))
			return false;
	#endif
	static string[256];
	format(string, sizeof(string), "youtubeinmp3.com/fetch/?format=text&video=%s", url);
	HTTP(playerid, HTTP_GET, string, "", "YoutubeResponse");
	youtube_posX[playerid]		= posX;
	youtube_posY[playerid]		= posY;
	youtube_posZ[playerid]		= posZ;
	youtube_distance[playerid]	= distance;
	youtube_usepos[playerid]	= usepos;
	return true;
}