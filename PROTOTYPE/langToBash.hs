import System.Environment
import System.IO
import Text.Parsec.String

showUsage :: IO ()
showUsage = do System.IO.hPutStr stderr
                ("Usage    : langToBash <file>\n" ++
		"Sun Feb 16 15:55:08 JST 2014\n" )

main :: IO()
main = do args <- getArgs
          case args of
               []  -> showUsage
               [f] -> readF f >>= print
               _   -> showUsage

readF :: String -> IO String
readF "-" = getContents
readF f   = readFile f
