from numba import njit

# from jitq.pretty_print import parseprint
from ast import *
import jitq.ast_shortcuts as ast_scuts


def ast_decorator(func):
    def wrapper(self, node):
        func(self, node)
        if node.successor is not None and self.bot_node != node:
            node.successor.accept(self)

    return wrapper


ROOT_HASH_TABLE = 'hash_table'
RESULT_LIST = 'result_list'
PARAMETER_LIST = 'param_list'
PARAMETER_HASH_TABLE = 'param_hash_table'
FUNCTION_NAME = 'f'


class ASTGenerator:
    def __init__(self, stage_ast, top_node, bot_node):
        self.stage_ast = stage_ast
        self.top_node = top_node
        self.bot_node = bot_node

    def generate_ast(self):
        """generate ast from top to the bot node inclusive"""
        # setup ast
        body_ast = []
        func_def = Module(
            body=[FunctionDef(name=FUNCTION_NAME, args=arguments(
                args=[arg(arg=PARAMETER_LIST, annotation=None), arg(arg=PARAMETER_HASH_TABLE, annotation=None)],
                vararg=None,
                kwonlyargs=[], kw_defaults=[], kwarg=None, defaults=[NameConstant(value=None)]),
                              body=body_ast, decorator_list=[], returns=None)])

        inner_ast = []
        body_ast.append(
            For(target=Name(id=self.stage_ast.get_current_var(), ctx=Store()), iter=Name(id=PARAMETER_LIST, ctx=Load()),
                body=inner_ast, orelse=[]))

        self.stage_ast.inner_ast = inner_ast
        self.stage_ast.root_ast = body_ast
        self.stage_ast.func_def = func_def

        self.top_node.accept(self)

    def add_final_statement_inner(self):
        self.stage_ast.root_ast.append(Expr(value=Yield(value=Name(id=self.stage_ast.get_current_var(), ctx=Load()))))

    def add_final_statement_root(self, stm):
        self.stage_ast.root_ast.append(stm)

    def prepend_statement_root(self, stm):
        self.stage_ast.root_ast.insert(0, stm)

    @ast_decorator
    def visit_map(self, node):
        print("visiting a map")
        # add the node function to the locals list
        self.stage_ast.locals[self.stage_ast.get_next_func_name()] = njit(node.func)
        current_var = self.stage_ast.get_current_var()
        self.stage_ast.append_inner_ast(
            Assign(targets=[Name(id=self.stage_ast.get_next_var(), ctx=Store())],
                   value=Call(func=Name(id=self.stage_ast.get_current_func_name(), ctx=Load()),
                              args=[Name(id=current_var, ctx=Load())], keywords=[])))

    @ast_decorator
    def visit_filter(self, node):
        print("visiting a filter")
        # add the node function to the locals list
        self.stage_ast.locals[self.stage_ast.get_next_func_name()] = njit(node.func)
        current_var = self.stage_ast.get_current_var()
        self.stage_ast.append_inner_ast(
            If(test=UnaryOp(op=Not(), operand=Call(func=Name(id=self.stage_ast.get_current_func_name(), ctx=Load()),
                                                   args=[Name(id=current_var, ctx=Load())], keywords=[])),
               body=[Continue()],
               orelse=[]))

    @ast_decorator
    def visit_flat_map(self, node):
        print("visiting a flatMap")
        # add the node function to the locals list
        self.stage_ast.locals[self.stage_ast.get_next_func_name()] = njit(node.func)
        current_var = self.stage_ast.get_current_var()
        # produce a new list
        self.stage_ast.append_inner_ast(
            Assign(targets=[Name(id=self.stage_ast.get_next_var(), ctx=Store())],
                   value=Call(func=Name(id=self.stage_ast.get_current_func_name(), ctx=Load()),
                              args=[Name(id=current_var, ctx=Load())], keywords=[])))
        # add new loop
        body_ast = []
        current_var = self.stage_ast.get_current_var()
        self.stage_ast.append_inner_ast(
            For(target=Name(id=self.stage_ast.get_next_var(), ctx=Store()), iter=Name(id=current_var, ctx=Load()),
                body=body_ast, orelse=[])
        )
        self.stage_ast.inner_ast = body_ast

    @ast_decorator
    def visit_join(self, node):
        raise NotImplemented


    @ast_decorator
    def visit_collection_source(self, node):
        values = node.values
        self.stage_ast.input_values = values


def _find_top_node(node):
    current_node = node
    while current_node.parent is not None:
        parent = current_node.parent
        parent.successor = current_node
        current_node = parent
    return current_node