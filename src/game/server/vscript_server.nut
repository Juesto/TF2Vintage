//========== Copyright © 2008, Valve Corporation, All rights reserved. ========
//
// Purpose:
//
//=============================================================================

local DoEntFire = DoEntFire
local DoEntFireByInstanceHandle = DoEntFireByInstanceHandle
local DoDispatchParticleEffect = DoDispatchParticleEffect
local DoUniqueString = DoUniqueString

function UniqueString( string = "" )
{
	return DoUniqueString( string.tostring() );
}

function EntFire( target, action, value = null, delay = 0.0, activator = null, caller = null )
{
	if ( !value )
	{
		value = "";
	}

	if ( "self" in this )
	{
		if ( !caller )
		{
			caller = self;
		}

		if ( !activator )
		{
			activator = self;
		}
	}

	return DoEntFire( target.tostring(), action.tostring(), value.tostring(), delay, activator, caller );
}

function EntFireByHandle( target, action, value = null, delay = 0.0, activator = null, caller = null )
{
	if ( !value )
	{
		value = "";
	}

	if ( "self" in this )
	{
		if ( !caller )
		{
			caller = self;
		}

		if ( !activator )
		{
			activator = self;
		}
	}

	return DoEntFireByInstanceHandle( target, action.tostring(), value.tostring(), delay, activator, caller );
}

function DispatchParticleEffect( particleName, origin, angles, entity = null )
{
	DoDispatchParticleEffect( particleName, origin, angles, entity );
}

function __ReplaceClosures( script, scope )
{
	if ( !scope )
	{
		scope = getroottable();
	}

	local tempParent = { getroottable = function() { return null; } };
	local temp = { runscript = script };
	delegate tempParent : temp;

	temp.runscript()
	foreach( key,val in temp )
	{
		if ( typeof(val) == "function" && key != "runscript" )
		{
			printl( "   Replacing " + key );
			scope[key] <- val;
		}
	}
}

local __OutputsPattern = regexp("^On.*Output$");

function ConnectOutputs( table )
{
	const nCharsToStrip = 6;
	foreach( key, val in table )
	{
		if ( typeof( val ) == "function" && __OutputsPattern.match( key ) )
		{
			//printl(key.slice( 0, nCharsToStrip ) );
			table.self.ConnectOutput( key.slice( 0, key.len() - nCharsToStrip ), key );
		}
	}
}

function IncludeScript( name, scope = null )
{
	if ( !scope )
	{
		scope = this;
	}
	return ::DoIncludeScript( name, scope );
}

//---------------------------------------------------------
// Text dump this scope's contents to the console.
//---------------------------------------------------------
function __DumpScope( depth, table )
{
	local indent=function( count )
	{
		local i;
		for( i = 0 ; i < count ; i++ )
		{
			print("   ");
		}
	}
	
    foreach(key, value in table)
    {
		indent(depth);
		print( key );
        switch (type(value))
        {
            case "table":
				print("(TABLE)\n");
				indent(depth);
                print("{\n");
                __DumpScope( depth + 1, value);
				indent(depth);
                print("}");
                break;
            case "array":
				print("(ARRAY)\n");
				indent(depth);
                print("[\n")
                __DumpScope( depth + 1, value);
				indent(depth);
                print("]");
                break;
            case "string":
                print(" = \"");
                print(value);
                print("\"");
                break;
            default:
                print(" = ");
                print(value);
                break;
        }
        print("\n");  
	}
}