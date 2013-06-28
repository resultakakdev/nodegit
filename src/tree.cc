/**
 * This code is auto-generated; unless you know what you're doing, do not modify!
 **/
#include <v8.h>
#include <node.h>
#include <string.h>

#include "git2.h"

#include "../include/tree.h"
#include "../include/repo.h"
#include "../include/oid.h"
#include "../include/tree_entry.h"

using namespace v8;
using namespace node;

GitTree::GitTree(git_tree *raw) {
  this->raw = raw;
}

GitTree::~GitTree() {
  git_tree_free(this->raw);
}

void GitTree::Initialize(Handle<v8::Object> target) {
  HandleScope scope;

  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);

  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  tpl->SetClassName(String::NewSymbol("Tree"));

  NODE_SET_PROTOTYPE_METHOD(tpl, "oid", Oid);
  NODE_SET_PROTOTYPE_METHOD(tpl, "size", Size);
  NODE_SET_PROTOTYPE_METHOD(tpl, "entryByName", EntryByName);
  NODE_SET_PROTOTYPE_METHOD(tpl, "entryByIndex", EntryByIndex);
  NODE_SET_PROTOTYPE_METHOD(tpl, "entryByOid", EntryByOid);
  NODE_SET_PROTOTYPE_METHOD(tpl, "getEntryByPath", GetEntryByPath);


  constructor_template = Persistent<Function>::New(tpl->GetFunction());
  target->Set(String::NewSymbol("Tree"), constructor_template);
}

Handle<Value> GitTree::New(const Arguments& args) {
  HandleScope scope;

  if (args.Length() == 0 || !args[0]->IsExternal()) {
    return ThrowException(Exception::Error(String::New("git_tree is required.")));
  }

  GitTree* object = new GitTree((git_tree *) External::Unwrap(args[0]));
  object->Wrap(args.This());

  return scope.Close(args.This());
}

Handle<Value> GitTree::New(void *raw) {
  HandleScope scope;
  Handle<Value> argv[1] = { External::New((void *)raw) };
  return scope.Close(GitTree::constructor_template->NewInstance(1, argv));
}

git_tree *GitTree::GetValue() {
  return this->raw;
}


Handle<Value> GitTree::Oid(const Arguments& args) {
  HandleScope scope;
  

  const git_oid * result = git_tree_id(
    ObjectWrap::Unwrap<GitTree>(args.This())->GetValue()
  );

  Handle<Value> to;
    to = GitOid::New((void *)result);
  return scope.Close(to);
}

Handle<Value> GitTree::Size(const Arguments& args) {
  HandleScope scope;
  

  size_t result = git_tree_entrycount(
    ObjectWrap::Unwrap<GitTree>(args.This())->GetValue()
  );

  Handle<Value> to;
    to = Uint32::New(result);
  return scope.Close(to);
}

Handle<Value> GitTree::EntryByName(const Arguments& args) {
  HandleScope scope;
    if (args.Length() == 0 || !args[0]->IsString()) {
    return ThrowException(Exception::Error(String::New("String filename is required.")));
  }

  String::Utf8Value filename(args[0]->ToString());
  const char * from_filename = strdup(*filename);

  const git_tree_entry * result = git_tree_entry_byname(
    ObjectWrap::Unwrap<GitTree>(args.This())->GetValue()
    , from_filename
  );
  delete from_filename;

  Handle<Value> to;
    to = GitTreeEntry::New((void *)result);
  return scope.Close(to);
}

Handle<Value> GitTree::EntryByIndex(const Arguments& args) {
  HandleScope scope;
    if (args.Length() == 0 || !args[0]->IsUint32()) {
    return ThrowException(Exception::Error(String::New("Number idx is required.")));
  }

  size_t from_idx = (size_t) args[0]->ToUint32()->Value();

  const git_tree_entry * result = git_tree_entry_byindex(
    ObjectWrap::Unwrap<GitTree>(args.This())->GetValue()
    , from_idx
  );

  Handle<Value> to;
    to = GitTreeEntry::New((void *)result);
  return scope.Close(to);
}

Handle<Value> GitTree::EntryByOid(const Arguments& args) {
  HandleScope scope;
    if (args.Length() == 0 || !args[0]->IsObject()) {
    return ThrowException(Exception::Error(String::New("Oid oid is required.")));
  }

  const git_oid * from_oid = ObjectWrap::Unwrap<GitOid>(args[0]->ToObject())->GetValue();

  const git_tree_entry * result = git_tree_entry_byoid(
    ObjectWrap::Unwrap<GitTree>(args.This())->GetValue()
    , from_oid
  );

  Handle<Value> to;
    to = GitTreeEntry::New((void *)result);
  return scope.Close(to);
}

Handle<Value> GitTree::GetEntryByPath(const Arguments& args) {
  HandleScope scope;
      if (args.Length() == 0 || !args[0]->IsString()) {
    return ThrowException(Exception::Error(String::New("String path is required.")));
  }

  if (args.Length() == 1 || !args[1]->IsFunction()) {
    return ThrowException(Exception::Error(String::New("Callback is required and must be a Function.")));
  }

  GetEntryByPathBaton* baton = new GetEntryByPathBaton;
  baton->error_code = GIT_OK;
  baton->error = NULL;
  baton->request.data = baton;
  baton->rootReference = Persistent<Value>::New(args.This());
  baton->root = ObjectWrap::Unwrap<GitTree>(args.This())->GetValue();
  baton->pathReference = Persistent<Value>::New(args[0]);
    String::Utf8Value path(args[0]->ToString());
  const char * from_path = strdup(*path);
  baton->path = from_path;
  baton->callback = Persistent<Function>::New(Local<Function>::Cast(args[1]));

  uv_queue_work(uv_default_loop(), &baton->request, GetEntryByPathWork, (uv_after_work_cb)GetEntryByPathAfterWork);

  return Undefined();
}

void GitTree::GetEntryByPathWork(uv_work_t *req) {
  GetEntryByPathBaton *baton = static_cast<GetEntryByPathBaton *>(req->data);
  int result = git_tree_entry_bypath(
    &baton->out, 
    baton->root, 
    baton->path
  );
  baton->error_code = result;
  if (result != GIT_OK) {
    baton->error = giterr_last();
  }
}

void GitTree::GetEntryByPathAfterWork(uv_work_t *req) {
  HandleScope scope;
  GetEntryByPathBaton *baton = static_cast<GetEntryByPathBaton *>(req->data);

  TryCatch try_catch;
  if (baton->error_code == GIT_OK) {
  Handle<Value> to;
    to = GitTreeEntry::New((void *)baton->out);
  Handle<Value> result = to;
    Handle<Value> argv[2] = {
      Local<Value>::New(Null()),
      result
    };
    baton->callback->Call(Context::GetCurrent()->Global(), 2, argv);
  } else if (baton->error) {
    Handle<Value> argv[1] = {
      Exception::Error(String::New(baton->error->message))
    };
    baton->callback->Call(Context::GetCurrent()->Global(), 1, argv);
  } else {
    baton->callback->Call(Context::GetCurrent()->Global(), 0, NULL);
  }

  if (try_catch.HasCaught()) {
    node::FatalException(try_catch);
  }
  baton->rootReference.Dispose();
  baton->pathReference.Dispose();
  baton->callback.Dispose();
  delete baton->path;
  delete baton;
}

Persistent<Function> GitTree::constructor_template;
