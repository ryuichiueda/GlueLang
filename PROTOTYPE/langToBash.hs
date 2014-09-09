import System.Environment
import System.Process
import System.IO
import Text.Parsec
import Text.Parsec.String
import qualified Data.Text as D
import qualified ToBash as Bash
import LangStructure

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
                    writeFile scrname $ (Bash.toBash . parseGlueLang) cs
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

parseGlueLang :: String -> Script
parseGlueLang str = case parse code "" str of
                          Right scr    -> scr 
                          Left err     -> Err ( show err )

code = do is <- many1 langImport
          many1 (string "\n")
          fs <- many1 langFilter
          return $ Script is fs

langImport = do string "import "
                p <- langWord
                string "as "
                a <- langWord
                char '\n'
                return $ Import p a

langFilter = do string "filter "
                nm <- langWord
		args <- many langWord
                many langSpace
                char ':'
                many1 ( char '\n' )
                lns <- many1 langFilterCode
                many (oneOf "\t\n")
                return $ Filter nm (zip [1..] args) lns

langWord = do w <- many1 (noneOf " :\n\t")
              many langSpace
              return w

langSpace = oneOf " \t"


langFilterCode = do char '\t'
                    ln <- many (noneOf "\n")
                    char '\n'
                    return ln
