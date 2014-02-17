import System.Environment
import System.IO
import Text.Parsec
import Text.Parsec.String
import qualified Data.Text as D
--import Text.ParserCombinators.Parsec

showUsage :: IO ()
showUsage = do System.IO.hPutStr stderr
                ("Usage    : langToBash <file>\n" ++
		"Sun Feb 16 15:55:08 JST 2014\n" )


type FilterName = String
type FilterArgs = (Int,String)
type FilterCode = String
data Filter = Filter FilterName [FilterArgs] [FilterCode] deriving Show
data Script = Script [Filter] | Err String deriving Show

main :: IO()
main = do args <- getArgs
          case args of
               []  -> showUsage
               [f] -> readF f >>= putStr . toBash . parseGlueLang
               _   -> showUsage

toBash :: Script -> String
toBash (Script fs) = unlines (header:(map toOneLiner fs) ++ [footer])
    where header = "#!/bin/bash -e\n"
          footer = "main " ++ mainArgs fs

mainArgs :: [Filter] -> String
mainArgs ((Filter "main" args _):fs) = unwords $ [ "\"" ++ ('$':(show n)) ++ "\"" | n <- [1..len]]
    where len = length args
mainArgs _ = ""

toOneLiner :: Filter -> String
toOneLiner (Filter fname opts codes) = func fname ++ "\n"
                                       ++ (pipeCon $ map (convArgs opts) codes) ++ "}"
         where func fname = "function " ++ fname ++ "(){"

pipeCon :: [String] -> String
pipeCon [s] = s ++ "\n"
pipeCon (s:ss) = s ++ " | " ++ pipeCon ss

convArgs :: [FilterArgs] -> FilterCode -> String
convArgs [] str = str
convArgs ((n,op):ops) str = convArgs ops $ D.unpack (D.replace (D.pack op) (D.pack $ ('$':show n)) (D.pack str))

readF :: String -> IO String
readF "-" = getContents
readF f   = readFile f

parseGlueLang :: String -> Script
parseGlueLang str = case parse code "" str of
                          Right scr    -> scr 
                          Left err     -> Err ( show err )

code = many1 langFilter >>= return . Script

langFilter = do string "filter "
                nm <- langWord
		args <- many langWord
                many langSpace
                char ':'
                many1 ( char '\n' )
                lns <- many1 langFilterCode
                return $ Filter nm (zip [1..] args) lns

langWord = do w <- many1 (noneOf " :\n\t")
              many langSpace
              return w

langSpace = oneOf " \t"

langFilterCode = do ln <- many (noneOf "\n")
                    char '\n'
                    return ln
