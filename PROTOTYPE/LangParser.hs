module LangParser( parseGlueLang ) where

import System.Environment
import System.Process
import System.IO
import Text.Parsec
import Text.Parsec.String
import qualified Data.Text as D
import qualified ToBash as Bash
import LangStructure

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
