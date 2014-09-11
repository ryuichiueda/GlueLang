module ToBash
( toBash
) where

import LangStructure
import qualified Data.Text as D

toBash :: Script -> String
toBash (Script is fs) = unlines ((map (blockToFunc is) fs) ++ [footer])
    where footer = mainArgs fs

mainArgs :: [Block] -> String
mainArgs ((Io "main" args _):fs) = unwords ("main" :opts) ++ " < /dev/stdin"
    where opts = [ "\"" ++ ('$':(show n)) ++ "\"" | n <- [1..(length args)]]
mainArgs ((Filter "main" args _):fs) = unwords ("main" :opts) ++ " < /dev/stdin"
    where opts = [ "\"" ++ ('$':(show n)) ++ "\"" | n <- [1..(length args)]]
mainArgs _ = ""

blockToFunc :: [Import] -> Block -> String
blockToFunc is (Io fname opts stats) = ioToFunc is fname opts stats
blockToFunc is (Filter fname opts stats) = filterToFunc is fname opts stats

ioToFunc :: [Import] -> Name -> [Args] -> [CommandLine] -> String
ioToFunc is name opts coms = "function " ++ name ++ "(){\n" ++ (unlines contents) ++ "}\n"
    where contents =  map ((\x -> ('\t':x)) . comToString is opts) coms

filterToFunc :: [Import] -> Name -> [Args] -> [CommandLine] -> String
filterToFunc is name opts coms = "function " ++ name ++ "(){\n\t" ++ contents ++ "}\n"
    where path_coms = map (solvePath is) coms
          contents = pipeCon $ map (convArgs opts) path_coms

solvePath :: [Import] -> CommandLine -> CommandLine
solvePath [] s     = s
solvePath ((Import path alias):is) (CommandLine fs (w:ws)) = if hit alias w 
                                                             then CommandLine fs ((path++com):ws)
                                                             else solvePath is (CommandLine fs (w:ws))
    where hit alias w = (alias ++ ".") == take (1 + length alias) w
          com = drop (1 + length alias) w

comToString :: [Import] -> [Args] -> CommandLine -> String
comToString is as com = (convArgs as) . (solvePath is) . attachIo $ com

attachIo :: CommandLine -> CommandLine
attachIo (CommandLine [] ws) = CommandLine [] ws
attachIo (CommandLine ((Write,name):fs) ws) = CommandLine fs $ (var:(ws ++ [ "> /tmp/$$-" ++ name]))
    where var = name ++ "=/tmp/$$-" ++ name ++ "\n\t"
attachIo (CommandLine ((Str,name):fs) ws) = CommandLine fs $ ((name++"=$("):ws ++ [")"])

pipeCon :: [String] -> String
pipeCon [s]    = s ++ "\n"
pipeCon (s:ss) = s ++ " | " ++ pipeCon ss

convArgs :: [Args] -> CommandLine -> String
convArgs [] (CommandLine fs ss) = unwords ss
convArgs as (CommandLine fs ss) = unwords $ map (convArgs' as) ss

convArgs' :: [Args] -> String -> String
convArgs' [] str           = str
convArgs' ((n,op):ops) str = convArgs' ops $ D.unpack $
                             D.replace (D.pack ('$':op)) (D.pack $ ('$':show n)) (D.pack str)
