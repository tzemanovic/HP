import Data.Char (toLower)
import System.IO
import Data.List (intersperse)

type Name = String
type VarType = String
type VarName = String
type Include = String

{-|
Generates C++ source code for immutable struct including constructor and setters.

Example input:
immutableStruct "WindowConfig" [("unsigned", "width"), ("unsigned", "height"), ("WindowStyle", "windowStyle"), ("unsigned", "bitsPerPx")] ["windowStyle"]

Output:
#pragma once
#include "windowStyle.hpp"

// Generated using tools/immutableStruct.hs:
// gen "WindowConfig" [("unsigned", "width"), ("unsigned", "height"), ("WindowStyle", "windowStyle"), ("unsigned", "bitsPerPx")] ["windowStyle"]
namespace hp_fp
{
	struct WindowConfig
	{
	public:
		const unsigned _width;
		const unsigned _height;
		const WindowStyle _windowStyle;
		const unsigned _bitsPerPx;
	public:
		const WindowConfig width( const unsigned width ) const
		{
			return WindowConfig{ width, _height, _windowStyle, _bitsPerPx };
		}
		const WindowConfig height( const unsigned height ) const
		{
			return WindowConfig{ _width, height, _windowStyle, _bitsPerPx };
		}
		const WindowConfig windowStyle( const WindowStyle windowStyle ) const
		{
			return WindowConfig{ _width, _height, windowStyle, _bitsPerPx };
		}
		const WindowConfig bitsPerPx( const unsigned bitsPerPx ) const
		{
			return WindowConfig{ _width, _height, _windowStyle, bitsPerPx };
		}
	};
}

-}

gen :: Name -> [(VarType, VarName)] -> [Include] -> IO ()
gen n vs i = do
  outh <- openFile ((toLower (head n):tail n) ++ ".hpp") WriteMode
  hPutStrLn outh $ immutableStruct n vs i
  hClose outh

gen' :: Name -> [(VarType, VarName)] -> [Include] -> IO ()
gen' n vs i = putStrLn $ immutableStruct n vs i

immutableStruct :: Name -> [(VarType, VarName)] -> [Include] -> String
immutableStruct n vs i = "#pragma once\n" ++ includes i ++ "\n// Generated using tools/immutableStruct.hs:\n// gen " ++ args n vs i ++ "\nnamespace hp_fp\n{\nstruct " ++ n ++ "\n{\npublic:\n" ++ variables vs ++ "public:\n" ++ setters n (variableNames vs) vs ++ "};\n}"
  where 
  includes :: [Include] -> String
  includes i = foldl1 (++) (map (\x -> "#include \"" ++ x ++ ".hpp\"\n") i)
  args :: Name -> [(VarType, VarName)] -> [Include] -> String
  args n vs i = "\"" ++ n ++ "\" [" ++ vArgs vs ++ "] [" ++ foldl1 (++) (intersperse ", " (map (\x -> "\"" ++ x ++ "\"") i)) ++ "]"
    where
    vArgs :: [(String, String)] -> String
    vArgs [(vType, vName)] = "(\"" ++ vType ++ "\", \"" ++ vName ++ "\")"
    vArgs ((vType, vName):vs) = "(\"" ++ vType ++ "\", \"" ++ vName ++ "\")" ++ ", " ++ vArgs vs
  constructor :: String -> [(String, String)] -> String
  constructor _ [] = ""
  constructor n vs = n ++ "(" ++ constructorArgs vs ++ ");"
    where
    constructorArgs ::  [(String, String)] -> String
    constructorArgs [(vType, vName)] = "const " ++ vType ++ " " ++ vName
    constructorArgs ((vType, vName):vs) = "const " ++ vType ++ " " ++ vName ++ ", " ++ constructorArgs vs
  variables :: [(String, String)] -> String
  variables [] = ""
  variables ((vType, vName):vs) = "const " ++ vType ++ " _" ++ vName ++ ";\n" ++ variables vs
  setters :: String -> [String] -> [(String, String)] -> String
  setters _ _ [] = ""
  setters n vNames ((vType, vName):vs) = "const " ++ n ++ " " ++ vName ++ "(const " ++ vType ++ " " ++ vName ++ ") const\n{\nreturn " ++ n ++ "{ " ++ constructorArgs vName vNames ++ " };\n }\n" ++ setters n vNames vs
    where
    constructorArgs :: String -> [String] -> String
    constructorArgs currentVName [vName] = if (currentVName == vName) 
                                              then vName
                                              else "_" ++ vName
    constructorArgs currentVName (vName:vNames) = (if (currentVName == vName) 
                                                     then vName ++ "," 
                                                     else "_" ++ vName ++ ",")
                                                   ++ constructorArgs currentVName vNames
  variableNames :: [(String, String)] -> [String]
  variableNames [] = []
  variableNames ((vType, vName):vs) = vName : variableNames vs