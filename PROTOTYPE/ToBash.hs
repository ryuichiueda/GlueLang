module ToBash ( toBash ) where

import LangStructure
import qualified Data.Text as D

error_exit  = "ERROR_EXIT(){\n" ++ 
              "\trm -f /tmp/$$-*\n" ++
              "\texit 1\n" ++
              "}\n\n"

error_check = "ERROR_CHECK(){\n" ++ 
              "\t[ \"$(tr -d ' 0' <<< ${PIPESTATUS[@]})\" = \"\" ] && return\n" ++ 
              "\tERROR_EXIT\n" ++ 
              "}\n\n"

trap = "trap ERROR_EXIT 1 2 3 15\n\n"

eachline = "foreach(){\n\n" ++
    "\twhile read line ; do\n" ++
    "\t\t\"$1\" $line\n" ++
    "\t\tERROR_CHECK\n" ++
    "\tdone\n" ++
    --"\tERROR_CHECK\n" ++
    "}\n\n"

mktemp var = var ++"=$(mktemp /tmp/$$-" ++ var ++ "-XXXXXX" ++ ")\nERROR_CHECK\n"
rmfiles = "rm -f /tmp/$$-*"
coverBashFunc name code = name ++ "(){\n" ++ code ++ "}\n"


toBash :: Script -> String
toBash (Script is fs) =  error_exit ++ error_check ++ trap ++ eachline ++
                         unlines (subblocks ++ (mainblock:maincall:rmfiles:[]))
    where isMain (Proc "main" _ _) = True
          isMain (Func "main" _ _) = True
          isMain _ = False
          subblocks = map (blockToBashFunc is) $ filter (\x -> isMain x == False) fs
          mainblock = blockToBashFunc is $ head $ filter isMain fs
          maincall = (head $ filter (/= "") $ map mainLine fs) ++ "\nERROR_CHECK\n"

mainLine :: InlineCmd -> String
mainLine (Proc "main" args _) = mainLine' (length args)
mainLine (Func "main" args _) = mainLine' (length args)
mainLine _ = ""

mainLine' :: Int -> String
mainLine' ns = "main " ++ (unwords $ map f [1..ns])
    where f n = "\"" ++ ('$':(show n)) ++ "\""

blockToBashFunc :: [Import] -> InlineCmd -> String
blockToBashFunc is (Proc fname opts sbs) = procToBashFunc is fname opts sbs
blockToBashFunc is (Func fname opts stats) = funcToBashFunc is fname opts stats


procToBashFunc :: [Import] -> Name -> [Args] -> [SubInlineCmd] -> String
procToBashFunc is name args [SubInlineCmd coms] = coverBashFunc name code
    where code = unlines $ map (\x -> '\t':x ++ "\nERROR_CHECK\n") $ map (comToString is args) coms
procToBashFunc is name opts (sb:sbs) = coverBashFunc name code
    where code = (ifInlineCmd is opts sb) ++ (elifInlineCmd is opts sbs)

ifInlineCmd :: [Import] -> [Args] -> SubInlineCmd -> String
ifInlineCmd is args (IfInlineCmd cond coms) = "if " ++ (comToString is args cond) ++ " ; then\n\t"
                                            ++ (unlines $ map (comToString is args) coms)

elifInlineCmd :: [Import] -> [Args] -> [SubInlineCmd] -> String
elifInlineCmd is args ((IfInlineCmd (CommandLine _ ("othewise":[])) coms):[]) = "else\n\t"
    ++ (unlines $ map (comToString is args) coms) ++ "fi\n"
elifInlineCmd is args ((IfInlineCmd cond coms):[]) = "elif " ++ (comToString is args cond) ++ " ; then\n\t"
    ++ (unlines $ map (comToString is args) coms) ++ "fi\n"
elifInlineCmd is args ((IfInlineCmd cond coms):sbs) = "elif " ++ (comToString is args cond) ++ " ; then\n\t"
    ++ (unlines $ map (comToString is args) coms) ++ (elifInlineCmd is args sbs)

funcToBashFunc :: [Import] -> Name -> [Args] -> [CommandLine] -> String
funcToBashFunc is name opts coms = coverBashFunc name (contents ++ "\tERROR_CHECK\n")
    where contents = f $ map (comToString is opts) coms
          f [s]    = s ++ "\n"
          f (s:ss) = s ++ " |\n" ++ f ss

-- conversion from b.cat to /bin/cat
solvePath :: [Import] -> CommandLine -> CommandLine
solvePath [] s                    = s
solvePath is (CommandLine ios ws) = CommandLine ios $ map (solvePath' is) ws

solvePath' :: [Import] -> String -> String
solvePath' [] w = w
solvePath' ((Import path alias):is) w = if hit alias w then (path++com) else solvePath' is w
    where hit a s = (a ++ ".") == take (1 + length a) s
          com = drop (1 + length  alias) w


comToString :: [Import] -> [Args] -> CommandLine -> String
comToString is as (Heredoc (Write,name) s) = heredoc name s
comToString is as com = (convArgs as) . (solvePath is) $ addIo com

heredoc :: String -> String -> String
heredoc name contents = (mktemp name) ++ header ++ contents ++ "EOF"
    where header = "cat << \'EOF\' >> " ++ "$" ++ name ++ "\n"

addIo :: CommandLine -> CommandLine
addIo (CommandLine [] ws) = CommandLine [] ws
addIo (CommandLine (f:fs) ws) = addIo ( CommandLine fs (addIo' f ws) )

addIo' :: IoObj -> [String] -> [String]
addIo' (Write,name) ws = (mktemp name):(ws ++ [ "> $" ++ name])
addIo' (Str,name)   ws = [name++"=$("] ++ ws ++ [")"]

convArgs :: [Args] -> CommandLine -> String
convArgs [] (CommandLine fs ss) = unwords ss
convArgs as (CommandLine fs ss) = unwords $ map (f as) ss
    where f [] str           = str
          f ((n,op):ops) str = f ops $ D.unpack  
                               $ D.replace (D.pack ('$':op)) (D.pack $ ('$':show n)) (D.pack str)
