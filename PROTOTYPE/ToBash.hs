module ToBash
( toBash
) where

import LangStructure
import qualified Data.Text as D

toBash :: Script -> String
toBash (Script is fs) = unlines ((map (blockToFunc is) fs) ++ [footer])
    where footer = mainArgs fs

{--
header :: [Import] -> String
header is = (getpath $ head $ filter def is) ++ "\n"
    where def (Import path alias) = alias == "default"
          getpath (Import path alias) = path
--}

mainArgs :: [Block] -> String
mainArgs ((Io "main" args _):fs) = unwords ("main" :opts) ++ " < /dev/stdin"
    where opts = [ "\"" ++ ('$':(show n)) ++ "\"" | n <- [1..(length args)]]
mainArgs _ = ""

blockToFunc :: [Import] -> Block -> String
blockToFunc is (Io fname opts stats) = ioToFunc is fname opts stats
blockToFunc is (Filter fname opts stats) = filterToFunc is fname opts stats

ioToFunc :: [Import] -> Name -> [Args] -> [Statement] -> String
ioToFunc is name opts stats = "function " ++ name ++ "(){\n\t" ++ contents ++ "}\n"
    where path_stats = map (solvePath is) stats
          contents = pipeCon $ map (convArgs opts) path_stats

filterToFunc :: [Import] -> Name -> [Args] -> [Statement] -> String
filterToFunc is name opts stats = "function " ++ name ++ "(){\n\t" ++ contents ++ "}\n"
    where path_stats = map (solvePath is) stats
          contents = pipeCon $ map (convArgs opts) path_stats

solvePath :: [Import] -> Statement -> Statement
solvePath [] s     = s
solvePath ((Import path alias):is) (Statement (w:ws)) = if hit alias w 
                                                        then Statement ((path++com):ws)
                                                        else solvePath is (Statement (w:ws))
    where hit alias w = (alias ++ ".") == take (1 + length alias) w
          com = drop (1 + length alias) w
                                                     
--solvePath is (Statement (com:as)) = Statement (com:as)
--    where w = takeWhile (/= '.') com


pipeCon :: [String] -> String
pipeCon [s]    = s ++ "\n"
pipeCon (s:ss) = s ++ " | " ++ pipeCon ss

convArgs :: [Args] -> Statement -> String
convArgs [] (Statement ss) = unwords ss
convArgs as (Statement ss) = unwords $ map (convArgs' as) ss

convArgs' :: [Args] -> String -> String
convArgs' [] str           = str
convArgs' ((n,op):ops) str = convArgs' ops $ D.unpack $
                             D.replace (D.pack ('$':op)) (D.pack $ ('$':show n)) (D.pack str)
