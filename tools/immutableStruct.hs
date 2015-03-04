import Data.Char (toLower, toUpper)
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
// gen "WindowConfig" [("UInt", "width"), ("UInt", "height"), ("WindowStyle", "windowStyle"), ("UInt", "bitsPerPx")] ["windowStyle"]
namespace hp_fp
{
	struct WindowConfigImm
	{
		const UInt width;
		const UInt height;
		const WindowStyle windowStyle;
		const UInt bitsPerPx;
		const WindowConfigImm setWidth( const UInt w ) const
		{
			return WindowConfigImm{ w, height, windowStyle, bitsPerPx };
		}
		const WindowConfigImm setHeight( const UInt h ) const
		{
			return WindowConfigImm{ width, h, windowStyle, bitsPerPx };
		}
		const WindowConfigImm setWindowStyle( const WindowStyle w ) const
		{
			return WindowConfigImm{ width, height, w, bitsPerPx };
		}
		const WindowConfigImm setBitsPerPx( const UInt b ) const
		{
			return WindowConfigImm{ width, height, windowStyle, b };
		}
	};
}

-}

gen :: Name -> [(VarType, VarName)] -> [Include] -> IO ()
gen n vs i = do
  let filename = (toLower (head n):tail n) ++ "Imm.hpp"
  outh <- openFile filename WriteMode
  hPutStrLn outh $ immutableStruct n vs i
  hClose outh

gen' :: Name -> [(VarType, VarName)] -> [Include] -> IO ()
gen' n vs i = putStrLn $ immutableStruct n vs i

immutableStruct :: Name -> [(VarType, VarName)] -> [Include] -> String
immutableStruct n vs i = "#pragma once\n" ++ includes i ++ "// Generated using tools/immutableStruct.hs:\n// gen " ++ args n vs i ++ "\nnamespace hp_fp\n{\nstruct " ++ nn ++ "\n{\n" ++ variables vs ++ setters nn (variableNames vs) vs ++ "};\n}"
  where 
  nn = n ++ "Imm"
  includes :: [Include] -> String
  includes [] = ""
  includes i = foldl1 (++) (map (\x -> "#include <" ++ x ++ ">\n") i)
  args :: Name -> [(VarType, VarName)] -> [Include] -> String
  args n vs i = "\"" ++ n ++ "\" [" ++ vArgs vs ++ "] " ++ includesList i
    where
    vArgs :: [(String, String)] -> String
    vArgs [(vType, vName)] = "(\"" ++ vType ++ "\", \"" ++ vName ++ "\")"
    vArgs ((vType, vName):vs) = "(\"" ++ vType ++ "\", \"" ++ vName ++ "\")" ++ ", " ++ vArgs vs
    includesList :: [Include] -> String
    includesList [] = "[]"
    includesList i = "[" ++ foldl1 (++) (intersperse ", " (map (\x -> "\"" ++ x ++ "\"") i)) ++ "]"
  constructor :: String -> [(String, String)] -> String
  constructor _ [] = ""
  constructor n vs = n ++ "(" ++ constructorArgs vs ++ ");"
    where
    constructorArgs ::  [(String, String)] -> String
    constructorArgs [(vType, vName)] = "const " ++ vType ++ " " ++ vName
    constructorArgs ((vType, vName):vs) = "const " ++ vType ++ " " ++ vName ++ ", " ++ constructorArgs vs
  variables :: [(String, String)] -> String
  variables [] = ""
  variables ((vType, vName):vs) = "const " ++ vType ++ " " ++ vName ++ ";\n" ++ variables vs
  setters :: String -> [String] -> [(String, String)] -> String
  setters _ _ [] = ""
  setters n vNames ((vType, vName):vs) = "const " ++ n ++ " set" ++ (toUpper (head vName):tail vName) ++ "(const " ++ vType ++ " " ++ [head vName] ++ ") const\n{\nreturn " ++ n ++ "{ " ++ constructorArgs vName vNames ++ " };\n }\n" ++ setters n vNames vs
    where
    constructorArgs :: String -> [String] -> String
    constructorArgs currentVName [vName] = if (currentVName == vName) 
                                              then [head vName]
                                              else vName
    constructorArgs currentVName (vName:vNames) = (if (currentVName == vName) 
                                                     then [head vName] ++ ", " 
                                                     else vName ++ ", ")
                                                   ++ constructorArgs currentVName vNames
  variableNames :: [(String, String)] -> [String]
  variableNames [] = []
  variableNames ((vType, vName):vs) = vName : variableNames vs