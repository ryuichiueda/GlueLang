module ToBash
( toBash
) where

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

trap = "trap ERROR_EXIT 2\n\n"

eachline = "foreach(){\n\n" ++
    "\twhile read line ; do\n" ++
    "\t\t\"$1\" $line\n" ++
    "\t\tERROR_CHECK\n" ++
    "\tdone\n" ++
    "\tERROR_CHECK\n" ++
    "}\n\n"

toBash :: Script -> String
toBash (Script is fs) =  error_exit ++ error_check ++ trap ++ eachline ++
                         unlines (subblocks ++ [mainblock] ++ [footer])
    where footer = head $ filter (/= "") $ map mainArgs fs
          isMain (Proc "main" _ _) = True
          isMain (Func "main" _ _) = True
          isMain _ = False
          subblocks = map (blockToFunc is) $ filter (\x -> isMain x == False) fs
          mainblock = blockToFunc is $ head $ filter isMain fs

mainArgs :: InlineCmd -> String
mainArgs (Proc "main" args _) = unwords ("main" :opts) -- ++ " < /dev/stdin"
    where opts = [ "\"" ++ ('$':(show n)) ++ "\"" | n <- [1..(length args)]]
mainArgs (Func "main" args _) = unwords ("main" :opts) ++ " < /dev/stdin"
    where opts = [ "\"" ++ ('$':(show n)) ++ "\"" | n <- [1..(length args)]]
mainArgs _ = ""

blockToFunc :: [Import] -> InlineCmd -> String
blockToFunc is (Proc fname opts sbs) = ioToFunc is fname opts sbs
blockToFunc is (Func fname opts stats) = filterToFunc is fname opts stats

--data SubInlineCmd = IfInlineCmd CommandLine [CommandLine] | SubInlineCmd [CommandLine] deriving Show
ioToFunc :: [Import] -> Name -> [Args] -> [SubInlineCmd] -> String
ioToFunc is name args [SubInlineCmd coms] = name ++ "(){\n"
                                         ++ (unlines $ map (comToString is args) coms)
                                         ++ "}\n"
ioToFunc is name opts (sb:sbs) = "function " ++ name ++ "(){\n"
                                 ++ (ifInlineCmd is opts sb)
                                 ++ (elifInlineCmd is opts sbs)
                                 ++ "}\n"

ifInlineCmd :: [Import] -> [Args] -> SubInlineCmd -> String
ifInlineCmd is args (IfInlineCmd cond coms) = "if " ++ (comToString is args cond) ++ " ; then\n\t"
                                            ++ (unlines $ map (comToString is args) coms)

elifInlineCmd :: [Import] -> [Args] -> [SubInlineCmd] -> String
elifInlineCmd is args ((IfInlineCmd (CommandLine _ ("othewise":[])) coms):[]) = "else\n\t"
                                               ++ (unlines $ map (comToString is args) coms)
                                               ++ "fi\n"
elifInlineCmd is args ((IfInlineCmd cond coms):[]) = "elif " ++ (comToString is args cond) ++ " ; then\n\t"
                                               ++ (unlines $ map (comToString is args) coms)
                                               ++ "fi\n"
elifInlineCmd is args ((IfInlineCmd cond coms):sbs) = "elif " ++ (comToString is args cond) ++ " ; then\n\t"
                                               ++ (unlines $ map (comToString is args) coms)
                                               ++ (elifInlineCmd is args sbs)


filterToFunc :: [Import] -> Name -> [Args] -> [CommandLine] -> String
filterToFunc is name opts coms = name ++ "(){\n\t" 
                                 ++ contents
                                 ++ "\tERROR_CHECK\n"
                                 ++ "}\n"
    where contents = pipeCon $ map (comToString is opts) coms

solvePath :: [Import] -> CommandLine -> CommandLine
solvePath [] s                    = s
solvePath is (CommandLine ios ws) = CommandLine ios $ map (solvePath' is) ws

-- conversion from b.cat to /bin/cat
solvePath' :: [Import] -> String -> String
solvePath' [] w = w
solvePath' ((Import path alias):is) w = if hit alias w then (path++com) else solvePath' is w
    where hit a s = (a ++ ".") == take (1 + length a) s
          com = drop (1 + length alias) w


comToString :: [Import] -> [Args] -> CommandLine -> String
comToString is as (Heredoc (Write,name) s) = name ++ "=" ++ file ++ "\n"
                                             ++ "cat << \'EOF\' >> " ++ file ++ "\n" ++ s ++ "EOF"
    where file = "/tmp/$$-" ++ name
comToString is as com = (convArgs as) . (solvePath is) . attachProc $ com

attachProc :: CommandLine -> CommandLine
attachProc (CommandLine [] ws) = CommandLine [] ws
attachProc (CommandLine ((Write,name):fs) ws) = attachProc $ CommandLine fs $ (var:(ws ++ [ "> /tmp/$$-" ++ name]))
    where var = name ++ "=/tmp/$$-" ++ name ++ "\n\t"
attachProc (CommandLine ((Str,name):fs) ws) = attachProc $ CommandLine fs $ [name++"=$("] ++ ws ++ [")"]

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
