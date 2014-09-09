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
                    writeFile scrname $ (Bash.toBash . P.parseGlueLang) cs
{--
an attempt of automatic execution of the generated bash script
This function doesn't work well since
the standard input is buffered before on memory. 
The amount of buffer is small.
                    let opts = ["-evx",scrname] ++ as
                    (stdin, stdout, stderr ,procHandle) <- runInteractiveProcess  "bash" opts Nothing Nothing
                    hPutStr stdin =<< getContents
                    hFlush stdin
                    hClose stdin
                    putStr =<< hGetContents stdout
--}
                
readF :: String -> IO String
readF "-" = getContents
readF f   = readFile f
