module LangStructure
( Name
, Arg
, CmdLine(..)
, SubCmd(..)
, Import(..)
, InOut(..)
, Io
, SubSubCmd(..)
, Script(..)
) where

type Name = String
type Arg = (Int,String)
type Path = String
type Alias = String
type Type = String
type Entity = String
type Io = (InOut,Name) 
data InOut = Read | Write | Append | Str deriving Show

data Script = Script [Import] [SubCmd] 
            | Err String deriving Show

data Import = Import Path Alias deriving Show

data SubCmd = Func Name [Arg] [CmdLine] 
            | Proc Name [Arg] [SubSubCmd] 
            | Test Name [Arg] [SubSubCmd] deriving Show

data SubSubCmd = IfSubCmd CmdLine [CmdLine] 
               | SubSubCmd [CmdLine] deriving Show

data CmdLine = CmdLine [Io] [String] 
                 | Heredoc Io String deriving Show
