module LangParser( parseGlueLang ) where

import Text.Parsec
import Text.Parsec.String
import LangStructure

parseGlueLang :: String -> Script
parseGlueLang str = case parse code "" str of
                          Right scr    -> scr 
                          Left err     -> Err ( show err )

code = do is <- many1 langImport
          many1 (string "\n")
          fs <- many1 (try(langFilter) <|> try(langIo))
          return $ Script is fs


langImport = do string "import "
                p <- langWord
                string "as "
                a <- langWord
                char '\n'
                return $ Import p a

langIo     = do string "io "
                nm <- langWord
		args <- many langWord
                many langSpace
                char ':'
                many1 ( char '\n' )
                lns <- many1 langCommandLine
                many (oneOf "\t\n")
                return $ Io nm (zip [1..] args) lns

langFilter = do string "filter "
                nm <- langWord
		args <- many langWord
                many langSpace
                char ':'
                many1 ( char '\n' )
                lns <- many1 langCommandLineNoIo
                many (oneOf "\t\n")
                return $ Filter nm (zip [1..] args) lns

langWord = do w <- many1 (noneOf " :\n\t")
              many langSpace
              return w

langSpace = oneOf " \t"

langCommandLine = try(langCommandLineOutfile) <|> try(langCommandLineOutstr) <|> try(langCommandLineNoIo)

langCommandLineOutfile = do string "\tfile "
                            many $ char ' '
                            f <- langWord
                            many $ char ' '
                            char '='
                            many $ char ' '
                            ln <- many1 langWord
                            char '\n'
                            return $ CommandLine [(Write,f)] ln

langCommandLineOutstr  = do string "\tstr "
                            many $ char ' '
                            f <- langWord
                            many $ char ' '
                            char '='
                            many $ char ' '
                            ln <- many1 langWord
                            char '\n'
                            return $ CommandLine [(Str,f)] ln

langCommandLineNoIo = do char '\t'
                         ln <- many1 langWord
                         char '\n'
                         return $ CommandLine [] ln
