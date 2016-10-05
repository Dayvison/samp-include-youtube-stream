#include <a_samp>
#include <core>
#include <audio>
#include <youtube_stream>

#pragma tabsize 0

main()
{
	print("\n----------------------------------");
	print("  Bare Script\n");
	print("----------------------------------\n");
}

public OnPlayerConnect(playerid)
{
	GameTextForPlayer(playerid,"~w~SA-MP: ~r~Bare Script",5000,5);
	return 1;
}
const Dialog_3 = 1;
public OnPlayerCommandText(playerid, cmdtext[])
{
	new idx;
	new str[256];

	str = strtok(cmdtext, idx);
	if(strcmp(str, "/play", true) == 0) {
		str = strtok(cmdtext, idx);
		if(PlayYoutubeForPlayer(playerid, str))
			SendClientMessage(playerid, -1, "wait for the music will start.");
		else
			SendClientMessage(playerid, -1, "Invalid url.");
    	return 1;
	}

	return 0;
}
public OnPlayYoutubeUrl(playerid, title[], len, response)
{
	new string[156];
	if(response == 1)
	{
		format(string, sizeof(string), "A music %s, as started with %d seconds.", title, len);
		SendClientMessage(playerid, -1, string);
	}
	else
	{
		SendClientMessage(playerid, -1, "Error: have a fail with your music.");
	}
}
public OnPlayerSpawn(playerid)
{
	SetPlayerInterior(playerid,0);
	TogglePlayerClock(playerid,0);
	SendClientMessage(playerid, -1, "To test this include use:");
	SendClientMessage(playerid, -1, "{0000FF}/play <youtube-url>{FFFFFF} an youtube url will be played to you.");
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
   	return 1;
}

SetupPlayerForClassSelection(playerid)
{
 	SetPlayerInterior(playerid,14);
	SetPlayerPos(playerid,258.4893,-41.4008,1002.0234);
	SetPlayerFacingAngle(playerid, 270.0);
	SetPlayerCameraPos(playerid,256.0815,-43.0475,1004.0234);
	SetPlayerCameraLookAt(playerid,258.4893,-41.4008,1002.0234);
}

public OnPlayerRequestClass(playerid, classid)
{
	SetupPlayerForClassSelection(playerid);
	return 1;
}

public OnGameModeInit()
{
	SetGameModeText("Bare Script");
	ShowPlayerMarkers(1);
	ShowNameTags(1);
	AllowAdminTeleport(1);

	AddPlayerClass(265,1958.3783,1343.1572,15.3746,270.1425,0,0,0,0,-1,-1);

	return 1;
}

strtok(const string[], &index)
{
	new length = strlen(string);
	while ((index < length) && (string[index] <= ' '))
	{
		index++;
	}

	new offset = index;
	new result[156];
	while ((index < length) && (string[index] > ' ') && ((index - offset) < (sizeof(result) - 1)))
	{
		result[index - offset] = string[index];
		index++;
	}
	result[index - offset] = EOS;
	return result;
}
