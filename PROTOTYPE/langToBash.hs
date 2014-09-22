import System.Environment
import System.Process
import System.IO
import qualified ToBash as Bash
import qualified LangParser as P

showUsage :: IO ()
showUsage = do hPutStr stderr
                ("Usage    : langToBash <file>\n" ++
		"Sat Feb 22 13:08:28 JST 2014\n" )

main :: IO()
main = do args <- getArgs
          case args of
               []  -> showUsage
               _   -> main' args

main' :: [String] -> IO ()
main' (scr:as) = do cs <- readF scr
                    pn <- getProgName
                    let scrname = scr ++ ".bash"
                    putStr $ (Bash.toBash . P.parseGlueLang) cs
                
readF :: String -> IO String
readF "-" = getContents
readF f   = readFile f

