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

{--
langImportDefault = do string "import "
                       p <- langWord
                       string "as"
                       many1 (char ' ')
                       string "default\n"
                       return $ Import p ""
--}

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
                lns <- many1 langStatement
                many (oneOf "\t\n")
                return $ Io nm (zip [1..] args) lns

langFilter = do string "filter "
                nm <- langWord
		args <- many langWord
                many langSpace
                char ':'
                many1 ( char '\n' )
                lns <- many1 langStatement
                many (oneOf "\t\n")
                return $ Filter nm (zip [1..] args) lns

langWord = do w <- many1 (noneOf " :\n\t")
              many langSpace
              return w

langSpace = oneOf " \t"

langStatement = do char '\t'
                   ln <- many1 langWord
                   char '\n'
                   return $ Statement ln
